from HIPBOX_MIXER import HIPBOX_MIXER
from HIPBOX_MIXER import HIPBOX_CONNECT
from RECORD_MIXER import RECORD
from OSC_SERVER   import OSC
import threading


# -- Main Program --
def run():
    event = threading.Event()
    boxes = []
    # OSC.run()

    # for outp in output_ports:
        # boxes.append( HIPBOX_MIXER(
            # name    = outp["box_name"],
            # inports = input_ports,
            # outport = outp,
        # ).run() )

    # boxes.append( HIPBOX_CONNECT(connections).run() )

    record_inports = [
        {"name": "in_talkback", "pan": "C"}
    ]
    record_mix = []
    boxes.append( RECORD(record_inports, record_mix, None).run() )

    # -- Wait For Stop -- 
    print("Press Ctrl+C to stop")
    try:
        event.wait()
    except KeyboardInterrupt:
        for box in boxes: box.kill()
        print("\nInterrupted by user")



#  # -- Globals --
#  input_ports = []
#  for outp in ["james","jesse","mitch","drums"]:
#      for inp in [["james","L"],["jesse","R"],["mitch","C"],["drums","C"],["talkback","C"]]:
#          if inp[0] == outp:
#              input_ports.append( {"name": f"in_{inp[0]}", "osc_vol": f"{outp}_{inp[0]}_vol", "osc_mute": None, "osc_solo": None, "pan": inp[1]} )
#          elif inp[0] == "talkback":
#              input_ports.append( {"name": f"in_{inp[0]}", "osc_vol": f"{outp}_{inp[0]}_vol", "osc_mute": "talkback_toggle", "osc_solo": None, "pan": inp[1]} )
#          else:
#              input_ports.append( {"name": f"in_{inp[0]}", "osc_vol": f"{outp}_{inp[0]}_vol", "osc_mute": f"{outp}_{inp[0]}_mute", "osc_solo": f"{inp[0]}_{inp[0]}_solo", "pan": inp[1]} )
#  
#  output_ports = [
#      {"box_name": "james", "name": "out_james", "L": "system:playback_7", "R": "system:playback_8"},
#      {"box_name": "jesse", "name": "out_jesse", "L": "system:playback_9", "R": "system:playback_10"},
#      {"box_name": "mitch", "name": "out_mitch", "L": "system:playback_3", "R": "system:playback_4"},
#  ]
#  
#  connections = [
#          {"inport": "james_amp:out_0", "outport": "james_fx:in_0"},
#          {"inport": "jesse_amp:out_0", "outport": "jesse_fx:in_0"},
#          {"inport": "mitch_amp:out_0", "outport": "mitch_fx:in_0"},
#  
#          {"inport": "system:capture_1", "outport": "james_amp:in_0"},
#          {"inport": "system:capture_2", "outport": "jesse_amp:in_0"},
#          {"inport": "system:capture_3", "outport": "mitch_amp:in_0"},
#  
#          {"inport": "james_fx:out_0", "outport": "james_HBM:in_james"},
#          {"inport": "james_fx:out_0", "outport": "jesse_HBM:in_james"},
#          {"inport": "james_fx:out_0", "outport": "mitch_HBM:in_james"},
#  
#          {"inport": "jesse_fx:out_0", "outport": "james_HBM:in_jesse"},
#          {"inport": "jesse_fx:out_0", "outport": "jesse_HBM:in_jesse"},
#          {"inport": "jesse_fx:out_0", "outport": "mitch_HBM:in_jesse"},
#  
#          {"inport": "mitch_fx:out_0", "outport": "james_HBM:in_mitch"},
#          {"inport": "mitch_fx:out_0", "outport": "jesse_HBM:in_mitch"},
#          {"inport": "mitch_fx:out_0", "outport": "mitch_HBM:in_mitch"},
#  
#          {"inport": "system:capture_4", "outport": "james_HBM:in_talkback"},
#          {"inport": "system:capture_4", "outport": "jesse_HBM:in_talkback"},
#          {"inport": "system:capture_4", "outport": "mitch_HBM:in_talkback"},
#  
#          {"inport": "system:capture_5", "outport": "james_HBM:in_drums"},
#          {"inport": "system:capture_5", "outport": "jesse_HBM:in_drums"},
#          {"inport": "system:capture_5", "outport": "mitch_HBM:in_drums"},
#  ]
#    
#  
#  
if __name__ == "__main__":
    run()
