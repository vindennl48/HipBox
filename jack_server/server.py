from pythonosc import osc_server
from pythonosc import udp_client
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
    "drums":    {"position": 4, "pan": "C", "inport": "system:capture_5"},
    "talkback": {"position": 5, "pan": "C", "inport": "system:capture_4"},
}

EXTRA_CONNECTIONS = [["system:midi_capture_1","mitch_guitarix:midi_in_1"]]

GUITARIX_START_PORT = 4000
MIXER_START_PORT    = 5000
OSC_INPORT          = 3001
OSC_OUTPORT         = 3002
OSC_VARS            = {}
IP                  = '127.0.0.1'
CLIENT              = None
# -- End Globals --


def run():
    start_client()
    start_guitarix()
    start_mixers()
    start_osc_server()
    start_extra_connections()


def start_osc_server():
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


def osc_callback(path, value):
    global OSC_VARS

    path       = path[1:]
    path_split = path.split("_")

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
            if pan in ["L","C"]: connect(inport,f"{name}_mix:in{pos}_left")
            if pan in ["R","C"]: connect(inport,f"{name}_mix:in{pos}_right")


def start_guitarix():
    for i, name in enumerate(PEOPLE):
        if PEOPLE[name]["guitarix"]:
            inport                        = PEOPLE[name]["inport"]
            guitarix_port                 = GUITARIX_START_PORT + i
            PEOPLE[name]["guitarix_port"] = guitarix_port

            os.system(f"guitarix -N -J -D --name={name}_guitarix -p {guitarix_port} &")
            time.sleep(1) # give guitarix a second to load
            connect(inport,f"{name}_guitarix:in_0")


def connect(port1, port2):
    success = False
    i = 0
    while not success and i < 5:
        try:
            CLIENT.connect(port1,port2)
            success = True
        except:
            print(f"####> error trying to connect ports attempt {i}-5: {port1} -> {port2}")
            i += 1
            time.sleep(1)
    if i >= 5:
        raise Exception(f">>>>> Can not connect ports in 5 attempts: {port1} -> {port2}")
    else:
        print(f"----> ports successfully connected: {port1} -> {port2}")


def start_client():
    global CLIENT
    CLIENT = jack.Client("server")
    CLIENT.activate()


def start_extra_connections():
    for c in EXTRA_CONNECTIONS:
        connect(c[0], c[1])


if __name__ == "__main__":
    run()
