from HIPBOX_MIXER import HIPBOX_VOLUME
from HIPBOX_MIXER import HIPBOX_CONNECT
from OSC_SERVER   import OSC
import threading


# -- Globals --
output_ports = {
    "mitch": {"L": "system:playback_3", "R": "system:playback_4"},
    "james": {"L": "system:playback_7", "R": "system:playback_8"},
    "jesse": {"L": "system:playback_9", "R": "system:playback_10"},
    "drums": {"L": "system:playback_5", "R": "system:playback_6"},
}
input_ports = {
    "james":    {"port": "system: capture_1", "pan": "L"},
    "jesse":    {"port": "system: capture_2", "pan": "R"},
    "mitch":    {"port": "system: capture_3", "pan": "C"},
    "talkback": {"port": "system: capture_4", "pan": "C"},
    "drums":    {"port": "system: capture_5", "pan": "C"},
}


def run():
    event = threading.Event()
    boxes = []

    # -- Create Boxes -- 
    for outp in output_ports:
        for inp in input_ports:
            solo = f"{outp}_{inp}_solo" if outp != inp else None
            mute = f"{outp}_{inp}_mute" if inp != "talkback" else "talkback_toggle"
            boxes.append( HIPBOX_VOLUME(
                f"{outp}_{inp}",
                f"{outp}_{inp}_vol",
                mute,
                solo,
            ) )
    boxes.append( HIPBOX_CONNECT(connections) )

    # -- Run Boxes -- 
    for box in boxes: box.run()

    # -- Wait For Stop -- 
    print("Press Ctrl+C to stop")
    try:
        event.wait()
    except KeyboardInterrupt:
        for box in boxes: box.kill()
        print("\nInterrupted by user")


# -- Create Connections --
connections = []
for outp in output_ports:
    for inp in input_ports:
        inport  = input_ports[inp]["port"]
        outport = f"{outp}_{inp}_HB:in_0"
        connections.append({ "inport": inport, "outport": outport })

        inport = f"{outp}_{inp}_HB:out_0"
        if input_ports[inp]["pan"] in ["L","C"]:
            outport = output_ports[outp]["L"]
            connections.append({ "inport": inport, "outport": outport })
        if input_ports[inp]["pan"] in ["R","C"]:
            outport = output_ports[outp]["R"]
            connections.append({ "inport": inport, "outport": outport })

  
#  connections_bup = [
#      # -- INPORTS --
#      { "inport": "system:capture_1",        "outport": "mitch_james_HB:in_0"},
#      { "inport": "system:capture_1",        "outport": "james_james_HB:in_0"},
#      { "inport": "system:capture_1",        "outport": "jesse_james_HB:in_0"},
#      { "inport": "system:capture_1",        "outport": "drums_james_HB:in_0"},
#  
#      { "inport": "system:capture_2",        "outport": "mitch_jesse_HB:in_0"},
#      { "inport": "system:capture_2",        "outport": "james_jesse_HB:in_0"},
#      { "inport": "system:capture_2",        "outport": "jesse_jesse_HB:in_0"},
#      { "inport": "system:capture_2",        "outport": "drums_jesse_HB:in_0"},
#  
#      { "inport": "system:capture_3",        "outport": "mitch_mitch_HB:in_0"},
#      { "inport": "system:capture_3",        "outport": "james_mitch_HB:in_0"},
#      { "inport": "system:capture_3",        "outport": "jesse_mitch_HB:in_0"},
#      { "inport": "system:capture_3",        "outport": "drums_mitch_HB:in_0"},
#  
#      { "inport": "system:capture_4",        "outport": "mitch_talkback_HB:in_0"},
#      { "inport": "system:capture_4",        "outport": "james_talkback_HB:in_0"},
#      { "inport": "system:capture_4",        "outport": "jesse_talkback_HB:in_0"},
#      { "inport": "system:capture_4",        "outport": "drums_talkback_HB:in_0"},
#  
#      { "inport": "system:capture_5",        "outport": "mitch_drums_HB:in_0"},
#      { "inport": "system:capture_5",        "outport": "james_drums_HB:in_0"},
#      { "inport": "system:capture_5",        "outport": "jesse_drums_HB:in_0"},
#      { "inport": "system:capture_5",        "outport": "drums_drums_HB:in_0"},
#  
#      # -- OUTPORTS --
#      { "inport": "mitch_james_HB:out_0",    "outport": "system:playback_3"},
#      { "inport": "james_james_HB:out_0",    "outport": "system:playback_7"},
#      { "inport": "jesse_james_HB:out_0",    "outport": "system:playback_9"},
#      { "inport": "drums_james_HB:out_0",    "outport": "system:playback_5"},
#  
#      { "inport": "mitch_jesse_HB:out_0",    "outport": "system:playback_4"},
#      { "inport": "james_jesse_HB:out_0",    "outport": "system:playback_8"},
#      { "inport": "jesse_jesse_HB:out_0",    "outport": "system:playback_10"},
#      { "inport": "drums_jesse_HB:out_0",    "outport": "system:playback_6"},
#  
#      { "inport": "mitch_mitch_HB:out_0",    "outport": "system:playback_3"},
#      { "inport": "mitch_mitch_HB:out_0",    "outport": "system:playback_4"},
#      { "inport": "james_mitch_HB:out_0",    "outport": "system:playback_7"},
#      { "inport": "james_mitch_HB:out_0",    "outport": "system:playback_8"},
#      { "inport": "jesse_mitch_HB:out_0",    "outport": "system:playback_9"},
#      { "inport": "jesse_mitch_HB:out_0",    "outport": "system:playback_10"},
#      { "inport": "drums_mitch_HB:out_0",    "outport": "system:playback_5"},
#      { "inport": "drums_mitch_HB:out_0",    "outport": "system:playback_6"},
#  
#      { "inport": "mitch_drums_HB:out_0",    "outport": "system:playback_3"},
#      { "inport": "mitch_drums_HB:out_0",    "outport": "system:playback_4"},
#      { "inport": "james_drums_HB:out_0",    "outport": "system:playback_7"},
#      { "inport": "james_drums_HB:out_0",    "outport": "system:playback_8"},
#      { "inport": "jesse_drums_HB:out_0",    "outport": "system:playback_9"},
#      { "inport": "jesse_drums_HB:out_0",    "outport": "system:playback_10"},
#      { "inport": "drums_drums_HB:out_0",    "outport": "system:playback_5"},
#      { "inport": "drums_drums_HB:out_0",    "outport": "system:playback_6"},
#  
#      { "inport": "mitch_talkback_HB:out_0", "outport": "system:playback_3"},
#      { "inport": "mitch_talkback_HB:out_0", "outport": "system:playback_4"},
#      { "inport": "james_talkback_HB:out_0", "outport": "system:playback_7"},
#      { "inport": "james_talkback_HB:out_0", "outport": "system:playback_8"},
#      { "inport": "jesse_talkback_HB:out_0", "outport": "system:playback_9"},
#      { "inport": "jesse_talkback_HB:out_0", "outport": "system:playback_10"},
#      { "inport": "drums_talkback_HB:out_0", "outport": "system:playback_5"},
#      { "inport": "drums_talkback_HB:out_0", "outport": "system:playback_6"},
#  ]
#  
#  if __name__ == "__main__":
#      run()
