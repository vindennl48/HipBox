from HIPBOX_MIXER import HIPBOX_MIXER
from HIPBOX_MIXER import HIPBOX_CONNECT
from OSC_SERVER   import OSC
import threading


# -- Globals --
input_ports = [
    {"name": "in_james",    "osc_vol": "mitch_james_vol",    "osc_mute": "mitch_james_mute", "osc_solo": "james_james_solo", "pan": "R"},
    {"name": "in_jesse",    "osc_vol": "mitch_jesse_vol",    "osc_mute": "mitch_jesse_mute", "osc_solo": "jesse_jesse_solo", "pan": "L"},
    {"name": "in_mitch",    "osc_vol": "mitch_mitch_vol",    "osc_mute": None,               "osc_solo": None,               "pan": "C"},
    {"name": "in_talkback", "osc_vol": "mitch_talkback_vol", "osc_mute": "talkback_toggle",  "osc_solo": None,               "pan": "C"},
    {"name": "in_drums",    "osc_vol": "mitch_drums_vol",    "osc_mute": "mitch_drums_mute", "osc_solo": "drums_drums_solo", "pan": "C"},
]
output_ports = [
    {"box_name": "mitch", "name": "out_mitch", "L": "system:playback_9", "R": "system:playback_10"},
]
connections = [
        # {"inport": "james_amp:out_0", "outport": "james_fx:in_0"},
        # {"inport": "jesse_amp:out_0", "outport": "jesse_fx:in_0"},
        # {"inport": "mitch_amp:out_0", "outport": "mitch_fx:in_0"},

        # {"inport": "system:capture_1", "outport": "james_amp:in_0"},
        # {"inport": "system:capture_2", "outport": "jesse_amp:in_0"},
        # {"inport": "system:capture_3", "outport": "mitch_amp:in_0"},

        # {"inport": "james_fx:out_0", "outport": "james_HBM:in_james"},
        # {"inport": "james_fx:out_0", "outport": "jesse_HBM:in_james"},
        {"inport": "james_fx:out_0", "outport": "mitch_HBM:in_james"},

        # {"inport": "jesse_fx:out_0", "outport": "james_HBM:in_jesse"},
        # {"inport": "jesse_fx:out_0", "outport": "jesse_HBM:in_jesse"},
        {"inport": "jesse_fx:out_0", "outport": "mitch_HBM:in_jesse"},

        # {"inport": "mitch_fx:out_0", "outport": "james_HBM:in_mitch"},
        # {"inport": "mitch_fx:out_0", "outport": "jesse_HBM:in_mitch"},
        {"inport": "mitch_fx:out_0", "outport": "mitch_HBM:in_mitch"},

        # {"inport": "system:capture_4", "outport": "james_HBM:in_talkback"},
        # {"inport": "system:capture_4", "outport": "jesse_HBM:in_talkback"},
        {"inport": "system:capture_4", "outport": "mitch_HBM:in_talkback"},

        # {"inport": "system:capture_5", "outport": "james_HBM:in_drums"},
        # {"inport": "system:capture_5", "outport": "jesse_HBM:in_drums"},
        {"inport": "system:capture_5", "outport": "mitch_HBM:in_drums"},
]


def run():
    event = threading.Event()
    boxes = []
    OSC.run()

    for outp in output_ports:
        boxes.append( HIPBOX_MIXER(
            name    = outp["box_name"],
            inports = input_ports,
            outport = outp,
        ).run() )

    boxes.append( HIPBOX_CONNECT(connections).run() )

    # -- Wait For Stop -- 
    print("Press Ctrl+C to stop")
    try:
        event.wait()
    except KeyboardInterrupt:
        for box in boxes: box.kill()
        print("\nInterrupted by user")
  


if __name__ == "__main__":
    run()
