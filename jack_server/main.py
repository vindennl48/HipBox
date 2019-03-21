from pythonosc import osc_server
from pythonosc import udp_client
from mixes import Mixes
from simple_daw import SimpleDAW
from guitarix import Guitarix
from midi_engine import MidiEngine
from recording_engine import RecordingEngine
import threading, argparse

# names and headphone outputs
PEOPLE = {
    "james": ["system:playback_7","system:playback_8"],
    "jesse": ["system:playback_9","system:playback_10"],
    "mitch": ["system:playback_3","system:playback_4"],
    "sean":  ["system:playback_5","system:playback_6"],
    "record":[],
}

# mixer inputs
INPUTS = {
    "james":    {"port": "james_guitarix:out_0", "pan": "L",         "record": True},
    # "jesse":    {"port": "system:capture_2",       "pan": "R",         "record": True},
    "jesse":    {"port": "jesse_guitarix:out_0", "pan": "R",         "record": True},
    # "mitch":    {"port": "mitch_guitarix:out_0", "pan": "C",         "record": True},
    "mitch":    {"port": "system:capture_3",     "pan": "C",         "record": True},
    "sean":     {"port": ["system:capture_5",    "system:capture_6", "SimpleDAW:out_0"], "pan": "C", "record": True},
    "talkback": {"port": "system:capture_4",     "pan": "C",         "record": False},
    "click":    {"port": "SimpleDAW:out_click",  "pan": "C",         "record": False},
}

GUITARIX_INPUTS = {
    "james": {"port": "system:capture_1"},
    "jesse": {"port": "system:capture_2"},
    # "mitch": {"port": "system:capture_3", "midi_port": "system:midi_capture_1"},
}

MIDI_MAP = {
    "note_6":  ["/rails/mute/mitch/talkback/toggle"],
    "note_10": [["/rails/record/mitch/record",0],"/simpledaw/stop/all","/record/stop"],

    # -- Sono --
    "note_7":  [["/simpledaw/timesig",3], ["/simpledaw/bpm",118], "/simpledaw/play/sono"],
    "note_8":  [["/simpledaw/timesig",3], ["/simpledaw/bpm",118], "/simpledaw/play/sono"],
    "note_9":  [["/simpledaw/timesig",3], ["/simpledaw/bpm",118], "/simpledaw/click"],

    # -- Chrono --
    # "note_17":  [["/simpledaw/timesig",4], ["/simpledaw/bpm",90], "/simpledaw/play/chrono"],
    # "note_18":  [["/simpledaw/timesig",4], ["/simpledaw/bpm",90], "/simpledaw/play/chrono"],
    "note_19":  [["/simpledaw/timesig",4], ["/simpledaw/bpm",90], "/simpledaw/click"],

    # -- Space --
    # "note_27":  [["/simpledaw/timesig",4], ["/simpledaw/bpm",120], "/simpledaw/play/space"],
    # "note_28":  [["/simpledaw/timesig",4], ["/simpledaw/bpm",120], "/simpledaw/play/space"],
    "note_29":  [["/simpledaw/timesig",4], ["/simpledaw/bpm",120], "/simpledaw/click"],

    # -- Petrichor --
    "note_37":  [["/simpledaw/timesig",4], ["/simpledaw/bpm",142], "/simpledaw/play/petrichor"],
    "note_38":  [["/simpledaw/timesig",4], ["/simpledaw/bpm",142], "/simpledaw/play/petrichor"],
    "note_39":  [["/simpledaw/timesig",4], ["/simpledaw/bpm",142], "/simpledaw/click"],

    # -- Blind --
    "note_47":  [["/simpledaw/timesig",3], ["/simpledaw/bpm",120], "/simpledaw/play/blind"],
    "note_48":  [["/simpledaw/timesig",3], ["/simpledaw/bpm",120], "/simpledaw/play/blind"],
    "note_49":  [["/simpledaw/timesig",3], ["/simpledaw/bpm",120], "/simpledaw/click"],

    # -- Blind --
    "note_57":  [["/simpledaw/timesig",4], ["/simpledaw/bpm",125], "/simpledaw/play/old_pete"],
    "note_58":  [["/simpledaw/timesig",4], ["/simpledaw/bpm",125], "/simpledaw/play/old_pete"],
    "note_59":  [["/simpledaw/timesig",4], ["/simpledaw/bpm",125], "/simpledaw/click"],
}

AUDIO_FILES = [
    {"name": "blind",     "gain": -10, "filepath": "/home/mitch/hipbox/audio_files/blind2.wav"},
    {"name": "sono",      "gain": -10, "filepath": "/home/mitch/hipbox/audio_files/sono2.wav"},
    {"name": "old_pete",  "gain": -10, "filepath": "/home/mitch/hipbox/audio_files/old_pete2.wav"},
    {"name": "petrichor", "gain": -10, "filepath": "/home/mitch/hipbox/audio_files/petrichor2.wav"},
]

CLICK_HIGH = {"name": "click_high", "gain": -16, "filepath": "/home/mitch/hipbox/audio_files/click_high.wav"}
CLICK_LOW  = {"name": "click_low",  "gain": -20, "filepath": "/home/mitch/hipbox/audio_files/click_low.wav"}

# Ports
OSC_INPORT          = 3001     # Receiving osc from rails
RAILS_OUTPORT       = 3002     # Sending osc to rails
GUITARIX_START_PORT = 4000
MIXER_START_PORT    = 5000
REC_START_PORT      = 6000
IP                  = '127.0.0.1'

# Clients
RAILS_CLIENT        = None

# Other Globals
class GLOBAL:
    # need to do it this way so the osc_callback
    #  function can actually use them.
    guitarix         = None
    mixes            = None
    simpledaw        = None
    midi_engine      = None
    recording_engine = None


parser = argparse.ArgumentParser()
parser.add_argument("--no-headless", action="store_false", help="Display Guitarix UI")
ARGS = parser.parse_args()


def run():
    # -- MAIN EVENT --
    GLOBAL.guitarix         = start_guitarix( isHeadless=ARGS.no_headless )
    GLOBAL.simpledaw        = start_simpledaw()
    GLOBAL.mixes            = start_mixes()
    GLOBAL.midi_engine      = start_midi_engine()
    GLOBAL.recording_engine = start_recording_engine()

    start_osc_server()
    load_user_mixer_presets()

    wait_for_interrupt()
    # -- #### --

def start_guitarix(isHeadless):
    return Guitarix(GUITARIX_INPUTS, GUITARIX_START_PORT, isHeadless=isHeadless)

def start_mixes():
    return Mixes(PEOPLE, INPUTS, MIXER_START_PORT, IP, RAILS_OUTPORT)

def start_simpledaw():
    return SimpleDAW(CLICK_HIGH, CLICK_LOW, AUDIO_FILES).run()

def start_midi_engine():
    connections = ["system:midi_capture_1"]
    return MidiEngine(connections, MIDI_MAP, OSC_INPORT, IP).run()

def start_recording_engine():
    filename_prepend = "recorded/scratch_"
    mixed_rec_ports  = ["record_mix:out_left","record_mix:out_right"]
    return RecordingEngine(INPUTS, filename_prepend, mixed_rec_ports, REC_START_PORT, IP)

def load_user_mixer_presets():
    if RAILS_CLIENT is not None:
        RAILS_CLIENT.send_message("/rails/maintenance/dump_vars", 0)

def rails_process_osc(path, value):
    path_sp = path[1:].split('/')

    # This only processes osc that are prefaced by 'simpledaw'
    if path_sp[0] != "rails": return 0

    if RAILS_CLIENT is not None:
        RAILS_CLIENT.send_message(path, value)
    return 1

def wait_for_interrupt():
    event = threading.Event()
    print("Press Ctrl+C to stop")
    try:
        event.wait()
    except KeyboardInterrupt:
        print("\nInterrupted by user")

def start_osc_server():
    global RAILS_CLIENT

    def osc_callback(path, value):
        print(f"----> OSC path: {path}, value: {value}")
        if rails_process_osc(path, value):
            return 1
        if GLOBAL.mixes is not None and GLOBAL.mixes.process_osc(path, value):
            return 1
        if GLOBAL.simpledaw is not None and GLOBAL.simpledaw.process_osc(path, value):
            return 1
        if GLOBAL.recording_engine is not None and GLOBAL.recording_engine.process_osc(path, value):
            return 1
        print("----> OSC missed")

    ########################################################
    # I hate importing inside a function but it doesn't seem
    #  to want to work any other way..
    from pythonosc import dispatcher
    ########################################################

    # accept osc data from rails
    disp = dispatcher.Dispatcher()
    disp.map("/*", osc_callback)
    server = osc_server.ThreadingOSCUDPServer((IP, OSC_INPORT), disp)
    threading.Thread(target=server.serve_forever).start()
    print(f"----> Listening on port udp:{OSC_INPORT}")

    # send osc data to rails
    RAILS_CLIENT = udp_client.SimpleUDPClient(IP, RAILS_OUTPORT)
    print(f"----> UDP client to rails active")


if __name__ == "__main__":
    run()
