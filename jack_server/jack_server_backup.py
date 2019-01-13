#!/usr/bin/env python3

from pythonosc import osc_server
import jack, threading, numpy

inputs = [
    {"system": "james_fx:out_0",   "hb": "james",    "pan":  "L"},
    {"system": "jesse_fx:out_0",   "hb": "jesse",    "pan":  "R"},
    {"system": "mitch_fx:out_0",   "hb": "mitch",    "pan":  "C"},
    {"system": "system:capture_4", "hb": "talkback", "pan":  "C"},
    {"system": "system:capture_5", "hb": "sean",     "pan":  "C"},
]

outputs = [
    {"system": ["system:playback_3", "system:playback_4"],  "hb": "mitch"},
    {"system": ["system:playback_7", "system:playback_8"],  "hb": "james"},
    {"system": ["system:playback_9", "system:playback_10"], "hb": "jesse"},
]

extra_connections = [
    # guitarix focusrite->head connections
    {"input": "system:capture_1",  "output": "james_amp:in_0"},
    {"input": "system:capture_2",  "output": "jesse_amp:in_0"},
    {"input": "system:capture_3",  "output": "mitch_amp:in_0"},

    # guitarix head->fx connections
    {"input": "james_amp:out_0", "output": "james_fx:in_0"},
    {"input": "jesse_amp:out_0", "output": "jesse_fx:in_0"},
    {"input": "mitch_amp:out_0", "output": "mitch_fx:in_0"},
]

class HIPBOX:
    gvars   = {}
    inputs  = {}
    outputs = {}
    event   = None
    client  = None

    def run(inputs, outputs, extra_connections):
        from pythonosc import dispatcher

        HIPBOX.client  = jack.Client("hipbox")
        HIPBOX.client.set_process_callback(HIPBOX._process_callback)
        HIPBOX.client.set_shutdown_callback(HIPBOX._shutdown_callback)

        # Register inputs and outputs with JACK
        for port in inputs:
            name = f"in_{port['hb']}"
            port['port'] = HIPBOX.client.inports.register(name)
            HIPBOX.inputs[name] = port

        for port in outputs:
            name = f"out_{port['hb']}"
            port['port'] = [
                HIPBOX.client.outports.register(name+"_L"),
                HIPBOX.client.outports.register(name+"_R"),
            ]
            HIPBOX.outputs[name] = port

        # Create the connection variables to change settings
        for o in outputs:
            for i in inputs:
                HIPBOX.gvars[f"{o['hb']}_{i['hb']}_vol"]  = 0
                HIPBOX.gvars[f"{o['hb']}_{i['hb']}_solo"] = 0
                HIPBOX.gvars[f"{o['hb']}_{i['hb']}_mute"] = 0

        # OSC SERVER
        ############
        def _callback(path, value):
            path = path[1:]
            if path in HIPBOX.gvars:
                if "vol" in path:
                    HIPBOX.gvars[path] = int((value * 40) - 30)
                else:
                    if value >= .5: HIPBOX.gvars[path] = 1
                    else:           HIPBOX.gvars[path] = 0
                print(f"path: {path} | value: {HIPBOX.gvars[path]}")

        disp = dispatcher.Dispatcher()
        disp.map("/*_*_*", _callback)
        server = osc_server.ThreadingOSCUDPServer(('0.0.0.0', 3001), disp)
        threading.Thread(target=server.serve_forever).start()
        ################
        # END OSC SERVER

        HIPBOX.event = threading.Event()
        with HIPBOX.client:
            for i in inputs:
                HIPBOX.client.connect(i['system'], f"hipbox:in_{i['hb']}")
            for o in outputs:
                HIPBOX.client.connect(f"hipbox:out_{o['hb']}_L", o['system'][0])
                HIPBOX.client.connect(f"hipbox:out_{o['hb']}_R", o['system'][1])
            for p in extra_connections:
                HIPBOX.client.connect(p['input'],p['output'])

            print("Press Ctrl+C to stop")
            try:
                HIPBOX.event.wait()
            except KeyboardInterrupt:
                print("\nInterrupted by user")

    def add_audio(destination, audio):
        if destination is None:
            destination = audio
        else:
            destination[:] = destination + audio
        return destination

    def _process_callback(frames):
        for outp_key in HIPBOX.outputs:
            output_array_L = None
            output_array_R = None
            for inp_key in HIPBOX.inputs:
                if HIPBOX.gvars[f"{HIPBOX.outputs[outp_key]['hb']}_{HIPBOX.inputs[inp_key]['hb']}_mute"]:
                    db = 0
                else:
                    db = (10 ** (HIPBOX.gvars[f"{HIPBOX.outputs[outp_key]['hb']}_{HIPBOX.inputs[inp_key]['hb']}_vol"] / 20) )
                if HIPBOX.inputs[inp_key]['pan'] in ['L','C']:
                    output_array_L = HIPBOX.add_audio(output_array_L, (HIPBOX.inputs[inp_key]['port'].get_array()*db) )
                if HIPBOX.inputs[inp_key]['pan'] in ['R','C']:
                    output_array_R = HIPBOX.add_audio(output_array_R, (HIPBOX.inputs[inp_key]['port'].get_array()*db) )
            HIPBOX.outputs[outp_key]['port'][0].get_array()[:] = output_array_L
            HIPBOX.outputs[outp_key]['port'][1].get_array()[:] = output_array_R

    def _shutdown_callback(status, reason):
        print("JACK shutdown!")
        print("status:", status)
        print("reason:", reason)
        HIPBOX.event.set()

HIPBOX.run(inputs, outputs, extra_connections)





#  #!/usr/bin/env python3
#  
#  from pythonosc import osc_server
#  import jack, threading, numpy
#  
#  class GVARS:
#      gvars = {}
#  
#  
#  passthrus = [
#      { "source": "system:capture_3", "input_name": "mitch", "pan": "C",
#          "output_name": "mitch_mitch", "sink": ["system:playback_7", "system:playback_8"] },
#      { "source": "system:capture_4", "input_name": "talkback", "pan": "C",
#          "output_name": "mitch_talkback", "sink": ["system:playback_7", "system:playback_8"] },
#  ]
#  
#  # OSC SERVER
#  ##############################
#  def osc_callback(path, value):
#      path = path[1:]
#      if path in GVARS.gvars:
#          if "vol" in path:
#              GVARS.gvars[path] = int((value * 40) - 30)
#          else:
#              if value >= .5: GVARS.gvars[path] = 1
#              else:           GVARS.gvars[path] = 0
#          print(f"path: {path} | value: {GVARS.gvars[path]}")
#  
#  disp = dispatcher.Dispatcher()
#  disp.map(f"/*_*_*", osc_callback)
#  server = osc_server.ThreadingOSCUDPServer(('0.0.0.0', 3001), disp)
#  threading.Thread(target=server.serve_forever).start()
#  ##############################
#  # OSC SERVER
#  
#  jack_name = "hipbox"
#  client    = jack.Client(jack_name)
#  
#  for pt in passthrus:
#      client.inports.register(pt['input_name'])
#      client.outports.register(pt['output_name']+"_L")
#      client.outports.register(pt['output_name']+"_R")
#      for n in ["_vol","_solo","_mute"]:
#          GVARS.gvars[pt['output_name']+"n"] = 0
#  
#  @client.set_process_callback
#  def process(frames):
#      for pt in passthrus:
#          if GVARS.gvars[f"{pt['output_name']}_mute"]:
#              self.out_L.get_array()[:] = self.in_M.get_array() * 0
#              self.out_R.get_array()[:] = self.in_M.get_array() * 0
#          else:
#              db = (10 ** (GVARS.gvars[f"{self.con['name']}_vol"] / 20) )
#              if self.con['pan'] in ['L','C']:
#                  self.out_L.get_array()[:] = self.in_M.get_array() * db
#              if self.con['pan'] in ['R','C']:
#                  self.out_R.get_array()[:] = self.in_M.get_array() * db
#  
#  client.activate()
#  for pt in passthrus:
#      client.connect(pt['source'], f"{jack_name}:{pt['input_name']}")
#      client.connect(f"{jack_name}:{pt['output_name']}_L", pt['sink'][0])
#      client.connect(f"{jack_name}:{pt['output_name']}_R", pt['sink'][1])
#  
#  
#  
#  
#  ################################################################################
#  
#  
#  
#  
#  class HIPBOX:
#      def __init__(self,connection):
#          from pythonosc import dispatcher
#  
#          self.con    = connection
#          self.client = jack.Client(self.con['name'])
#  
#          self.client.set_process_callback(self._process_callback)
#          self.client.set_shutdown_callback(self._shutdown_callback)
#  
#          self.in_M  = self.client.inports.register('in_M')
#          self.out_L = self.client.outports.register('out_L')
#          self.out_R = self.client.outports.register('out_R')
#          for n in ["_vol","_solo","_mute"]:
#              GVARS.gvars[self.con['name']+n]  = 0
#  
#          disp = dispatcher.Dispatcher()
#          disp.map(f"/{self.con['name']}_*", HIPBOX._osc_callback)
#          server = osc_server.ThreadingOSCUDPServer(('0.0.0.0', 3001), disp)
#          threading.Thread(target=server.serve_forever).start()
#  
#          self.client.activate()
#          self.client.connect(self.con['input'], f"{self.con['name']}:in_M")
#          self.client.connect(f"{self.con['name']}:out_L", self.con['output_L'])
#          self.client.connect(f"{self.con['name']}:out_R", self.con['output_R'])
#  
#      def _osc_callback(path, value):
#          path = path[1:]
#          if path in GVARS.gvars:
#              if "vol" in path:
#                  GVARS.gvars[path] = int((value * 40) - 30)
#              else:
#                  if value >= .5: GVARS.gvars[path] = 1
#                  else:           GVARS.gvars[path] = 0
#              print(f"path: {path} | value: {GVARS.gvars[path]}")
#  
#      def _process_callback(self, frames):
#          if GVARS.gvars[f"{self.con['name']}_mute"]:
#              self.out_L.get_array()[:] = self.in_M.get_array() * 0
#              self.out_R.get_array()[:] = self.in_M.get_array() * 0
#          else:
#              db = (10 ** (GVARS.gvars[f"{self.con['name']}_vol"] / 20) )
#              if self.con['pan'] in ['L','C']:
#                  self.out_L.get_array()[:] = self.in_M.get_array() * db
#              if self.con['pan'] in ['R','C']:
#                  self.out_R.get_array()[:] = self.in_M.get_array() * db
#  
#      def _shutdown_callback(self, status, reason):
#          print("JACK shutdown!")
#          print("status:", status)
#          print("reason:", reason)
#  
#  mitch_talkback = HIPBOX({
#      "name":     "mitch_talkback",
#      "input":    "system:capture_4",
#      "output_L": "system:playback_7",
#      "output_R": "system:playback_8",
#      "pan":      "C",
#  })
#  
#  mitch_mitch = HIPBOX({
#      "name":     "mitch_mitch",
#      "input":    "mitch_fx:out_0",
#      "output_L": "system:playback_7",
#      "output_R": "system:playback_8",
#      "pan":      "C",
#  })
#  
#  event = threading.Event()
#  print("Press Ctrl+C to stop")
#  try:
#      event.wait()
#  except KeyboardInterrupt:
#      print("\nInterrupted by user")
