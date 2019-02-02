from mixes import Mixes
from simple_daw import SimpleDAW
from guitarix import Guitarix

# names and headphone outputs
PEOPLE = {
    "james": ["system:playback_7","system:playback_8"],
    "jesse": ["system:playback_9","system:playback_10"],
    "mitch": ["system:playback_3","system:playback_4"],
    "sean":  ["system:playback_5","system:playback_6"],
}

# mixer inputs
INPUTS = {
    "james":    {"port": "james_guitarix:out_0", "pan": "L", "record": True},
    "jesse":    {"port": "jesse_guitarix:out_0", "pan": "R", "record": True},
    "mitch":    {"port": "mitch_guitarix:out_0", "pan": "C", "record": True},
    "talkback": {"port": "system:capture_4",     "pan": "C", "record": False},
    "click":    {"port": "SimpleDAW:out_click",  "pan": "C", "record": False},

    "sean": {"port": ["system:capture_5","system:capture_6"], "pan": "C", "record": True},
}

GUITARIX_INPUTS = {
    "james": {"port": "system:capture_1"},
    "jesse": {"port": "system:capture_2"},
    "mitch": {"port": "system:capture_3", "midi_port": "system:midi_capture_1"},
}

AUDIO_FILES = [
    {"name": "blind",     "filepath": "/home/mitch/hipbox/audio_files/blind2.wav"},
    {"name": "sono",      "filepath": "/home/mitch/hipbox/audio_files/sono2.wav"},
    {"name": "old_pete",  "filepath": "/home/mitch/hipbox/audio_files/old_pete2.wav"},
    {"name": "petrichor", "filepath": "/home/mitch/hipbox/audio_files/petrichor2.wav"},
]

CLICK_HIGH = {"name": "click_high", "filepath": "/home/mitch/hipbox/audio_files/click_high.wav"}
CLICK_LOW  = {"name": "click_low",  "filepath": "/home/mitch/hipbox/audio_files/click_low.wav"}

# Ports
OSC_INPORT           = 3001     # Receiving osc from rails
RAILS_OUTPORT        = 3002     # Sending osc to rails
GUITARIX_START_PORT  = 4000
MIXER_START_PORT     = 5000
REC_START_PORT       = 6000
IP                   = '127.0.0.1'


def run():
    event = threading.Event()

    # -- MAIN EVENT --
    guitarix  = start_guitarix()
    mixes     = start_mixes()
    simpledaw = start_simpledaw()
    # -- #### --

    print("Press Ctrl+C to stop")
    try:
        event.wait()
    except KeyboardInterrupt:
        print("\nInterrupted by user")


def start_guitarix():
    return Guitarix(GUITARIX_INPUTS, GUITARIX_START_PORT)


def start_mixes():
    return Mixes(PEOPLE, INPUTS, MIXER_START_PORT, IP, RAILS_OUTPORT)

def start_simpledaw():
    return SimpleDAW(CLICK_HIGH, CLICK_LOW, AUDIO_FILES)


if __name__ == "__main__":
    run()
