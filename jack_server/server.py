from pythonosc import osc_server
from pythonosc import udp_client
from audio_engine import AudioEngine
from midi_engine import MidiEngine
import jack, os, time, threading

# we need to take in osc messages from rails and translate
#  them to their respective ports.
# we also need to start the guitarix programs and link
#  all of the ports together.


# -- Globals --
PEOPLE = {
    "james": {"guitarix": True,  "inport": "system:capture_1", "outports": ["system:playback_7","system:playback_8"]},
    "jesse": {"guitarix": True,  "inport": "system:capture_2", "outports": ["system:playback_9","system:playback_10"]},
    "mitch": {"guitarix": True,  "inport": "system:capture_3", "outports": ["system:playback_3","system:playback_4"]},
    "drums": {"guitarix": False, "inport": "system:capture_5", "outports": ["system:playback_5","system:playback_6"]},
}

MIXER = {
    "james":    {"position": 1, "pan": "L", "inport": "james_guitarix:out_0"},
    "jesse":    {"position": 2, "pan": "R", "inport": "jesse_guitarix:out_0"},
    "mitch":    {"position": 3, "pan": "C", "inport": "mitch_guitarix:out_0"},
    "drums":    {"position": 4, "pan": "C", "inport": ["system:capture_5", "AudioEngine:out_0"]},
    "talkback": {"position": 5, "pan": "C", "inport": "system:capture_4"},
    "click":    {"position": 6, "pan": "C", "inport": "AudioEngine:out_click"},
}

MIDI_MAP = {
    "note_10": ["metronome_off", "stop_all"],
    "note_9":  ["timesig_3", "bpm_120", "metronome_toggle"],
    "note_8":  ["timesig_3", "bpm_120", "play_blind"],
}

EXTRA_CONNECTIONS = [["system:midi_capture_1","mitch_guitarix:midi_in_1"]]

# Ports
OSC_INPORT           = 3001  # Receiving osc from rails
OSC_OUTPORT          = 3002  # Sending osc to rails
GUITARIX_START_PORT  = 4000
MIXER_START_PORT     = 5000
AUDIO_ENGINE_PORT    = 6000
REC_START_PORT       = 7000
IP                   = '127.0.0.1'

# Clients
CLIENT               = None
REC_CLIENTS          = []
AUDIO_ENGINE         = None
MIDI_ENGINE          = None
RAILS_CLIENT         = None

# Vars
OSC_VARS             = {}
IS_RECORDING         = False

# Options
GUITARIX_IS_HEADLESS = True
# -- End Globals --


def run():
    event = threading.Event()

    # -- MAIN EVENT --
    start_client()
    start_audio_engine()
    start_midi_engine()
    start_guitarix()
    start_mixers()
    start_osc_server()
    start_extra_connections()
    # -- #### --

    print("Press Ctrl+C to stop")
    try:
        event.wait()
    except KeyboardInterrupt:
        print("\nInterrupted by user")


def start_osc_server():
    global RAILS_CLIENT

    ########################################################
    # I hate importing inside a function but it doesn't seem
    #  to want to work any other way..
    from pythonosc import dispatcher
    ########################################################

    disp = dispatcher.Dispatcher()
    disp.map("/*", osc_callback)
    server = osc_server.ThreadingOSCUDPServer((IP, OSC_INPORT), disp)
    threading.Thread(target=server.serve_forever).start()
    print(f"----> Listening on port udp:{OSC_INPORT}")


    RAILS_CLIENT = udp_client.SimpleUDPClient(IP, OSC_OUTPORT)
    print(f"----> UDP client to rails active")


def osc_callback(path, value):
    global OSC_VARS

    print(f"----> OSC: {path}, value: {value}")
    path       = path[1:]
    path_split = path.split("_")


    if path == "fcb_talkback_toggle":
        path       = "talkback_toggle"
        path_split = path.split("_")
        value = 1 if OSC_VARS[path] else 0

    if len(path_split) == 2:
        if path == "talkback_toggle":
            if value >= .5: value = 1
            else:           value = 0
            OSC_VARS[path] = value
            target         = "talkback"
            pos            = MIXER[target]["position"]

            for person in PEOPLE:
                client = PEOPLE[person]["udpclient"]
                vol    = f"{person}_{target}_vol"
                value  = OSC_VARS[vol] if vol in OSC_VARS else -90

                if  OSC_VARS[path]: value = -90
                client.send_message("/mixer/channel/set_gain", [pos, value])

        elif path == "record_toggle":
            start_rec()

        elif path == "stop_all":
            stop_rec()
            if RAILS_CLIENT is not None:
                print(f"####> STOP_ALL, path: /{path}")
                RAILS_CLIENT.send_message(f"/{path}", 0)
            if AUDIO_ENGINE is not None:
                AUDIO_ENGINE.osc_client.send_message(f"/{path}", 0)

        elif path_split[0] in ["play","stop","timesig","bpm","metronome"]:
            if AUDIO_ENGINE is not None:
                AUDIO_ENGINE.osc_client.send_message(f"/{path}", 0)

    elif len(path_split) == 3:
        name, target, kind = path_split

        if target in MIXER and name in PEOPLE:
            pos  = MIXER[target]["position"]
            mute = f"{name}_{target}_mute"
            solo = f"{target}_{target}_solo"

            if kind == "vol":
                client         = PEOPLE[name]["udpclient"]
                value          = int((value * 40) - 30)
                OSC_VARS[path] = value

                if "talkback" in path: mute = f"talkback_toggle"
                
                if  (mute in OSC_VARS and OSC_VARS[mute]) or \
                    (solo in OSC_VARS and OSC_VARS[solo]):
                    value = -90
                client.send_message("/mixer/channel/set_gain", [pos, value])

            elif kind == "mute":
                if value >= .5: value = 1
                else:           value = 0
                client         = PEOPLE[name]["udpclient"]
                OSC_VARS[path] = value
                vol            = f"{name}_{target}_vol"
                value          = OSC_VARS[vol] if vol in OSC_VARS else -90

                if  (mute in OSC_VARS and OSC_VARS[mute]) or \
                    (solo in OSC_VARS and OSC_VARS[solo]):
                    value = -90
                client.send_message("/mixer/channel/set_gain", [pos, value])

            elif kind == "solo":
                if value >= .5: value = 1
                else:           value = 0
                OSC_VARS[path] = value

                for person in PEOPLE:
                    if person != name:
                        client = PEOPLE[person]["udpclient"]
                        vol    = f"{person}_{target}_vol"
                        value  = OSC_VARS[vol] if vol in OSC_VARS else -90

                        if  (mute in OSC_VARS and OSC_VARS[mute]) or \
                            (solo in OSC_VARS and OSC_VARS[solo]):
                            value = -90
                        client.send_message("/mixer/channel/set_gain", [pos, value])


def start_mixers():
    num_ports = len(MIXER)

    for i, name in enumerate(PEOPLE):
        mixer_port = MIXER_START_PORT + i
        out_L      = PEOPLE[name]["outports"][0]
        out_R      = PEOPLE[name]["outports"][1]

        PEOPLE[name]["udpclient"] = udp_client.SimpleUDPClient(IP, mixer_port)

        os.system(f"jackminimix -c {num_ports} -n {name}_mix -p {mixer_port} -l {out_L} -r {out_R} &")
        time.sleep(1)  # give jackminimix a second to load
        for mix in MIXER:
            pos    = MIXER[mix]["position"]
            pan    = MIXER[mix]["pan"]
            inport = MIXER[mix]["inport"]
            if type(inport) is list:
                for i in inport:
                    if pan in ["L","C"]: connect(i,f"{name}_mix:in{pos}_left")
                    if pan in ["R","C"]: connect(i,f"{name}_mix:in{pos}_right")
            else:
                if pan in ["L","C"]: connect(inport,f"{name}_mix:in{pos}_left")
                if pan in ["R","C"]: connect(inport,f"{name}_mix:in{pos}_right")


def start_guitarix():
    for i, name in enumerate(PEOPLE):
        if PEOPLE[name]["guitarix"]:
            inport                        = PEOPLE[name]["inport"]
            guitarix_port                 = GUITARIX_START_PORT + i
            PEOPLE[name]["guitarix_port"] = guitarix_port

            headless = "-N"
            if not GUITARIX_IS_HEADLESS: headless = ""
            os.system(f"guitarix {headless} -J -D --name={name}_guitarix -p {guitarix_port} &")
            time.sleep(1) # give guitarix a second to load
            connect(inport,f"{name}_guitarix:in_0")


def connect(port1, port2):
    success = False
    i = 0
    while not success and i < 20:
        try:
            CLIENT.connect(port1,port2)
            success = True
        except:
            print(f"####> error trying to connect ports attempt {i}-20: {port1} -> {port2}")
            i += 1
            time.sleep(1)
    if i >= 20:
        raise Exception(f">>>>> Can not connect ports in 5 attempts: {port1} -> {port2}")
    else:
        print(f"----> ports successfully connected: {port1} -> {port2}")


def start_client():
    global CLIENT
    CLIENT = jack.Client("server")
    CLIENT.activate()


def start_audio_engine():
    global AUDIO_ENGINE

    # set up outboard connections
    connections = []
    # for name in PEOPLE:
        # for outp in PEOPLE[name]["outports"]:
            # connections.append(outp)

    AUDIO_ENGINE = AudioEngine(connections, osc_inport=AUDIO_ENGINE_PORT)
    AUDIO_ENGINE.load_audio(
        name     = "click_high",
        filepath = "/home/mitch/hipbox/audio_files/click_high.wav",
        start    = 0,
    )
    AUDIO_ENGINE.load_audio(
        name     = "click_low",
        filepath = "/home/mitch/hipbox/audio_files/click_low.wav",
        start    = 0,
    )

    AUDIO_ENGINE.load_audio(
        name     = "blind",
        filepath = "/home/mitch/hipbox/audio_files/blind2.wav",
        start    = 0,
    )
    AUDIO_ENGINE.run()


def start_midi_engine():
    global MIDI_ENGINE
    connections = ["system:midi_capture_1"]
    MIDI_ENGINE = MidiEngine(connections, MIDI_MAP, IP, OSC_INPORT).run()


def start_rec():
    global IS_RECORDING
    global REC_CLIENTS

    print("####> RECORDING")

    if not IS_RECORDING:

        IS_RECORDING = True
        port_offset  = 1
        make_clients = False

        if not REC_CLIENTS: make_clients = True

        for name in PEOPLE:
            port   = REC_START_PORT + port_offset
            inport = PEOPLE[name]['inport']

            if PEOPLE[name]['guitarix']:
                inport = f"{name}_guitarix:out_0"

            os.system(f"jack_capture -ns -b 16 -c 1 --osc {port} -mp3 -p {inport} -fp recorded/{name}_ &")

            if make_clients:
                REC_CLIENTS.append( udp_client.SimpleUDPClient(IP, port) )

            port_offset += 1

        os.system(f"jack_capture -ns -b 16 -c 2 --osc {REC_START_PORT} -mp3 -p mitch_mix:out_left -p mitch_mix:out_right -fp recorded/scratch_ &")
        if make_clients:
            REC_CLIENTS.append( udp_client.SimpleUDPClient(IP, REC_START_PORT) )


def stop_rec():
    global IS_RECORDING
    global REC_CLIENTS

    for client in REC_CLIENTS:
        client.send_message("/jack_capture/stop", [])
    IS_RECORDING = False


def start_extra_connections():
    for c in EXTRA_CONNECTIONS:
        connect(c[0], c[1])


if __name__ == "__main__":
    run()

    if AUDIO_ENGINE is not None:
        AUDIO_ENGINE.client.deactivate()
        AUDIO_ENGINE.client.close()
    if MIDI_ENGINE is not None:
        MIDI_ENGINE.client.deactivate()
        MIDI_ENGINE.client.close()
