#!/usr/bin/env python3

from pythonosc import osc_server
import jack, threading, numpy

class OSC:
    osc = {}

    def __init__(self):
        from pythonosc import dispatcher

        disp = dispatcher.Dispatcher()
        disp.map("/*_*_*", OSC._callback)
        server = osc_server.ThreadingOSCUDPServer(('0.0.0.0', 3001), disp)
        threading.Thread(target=server.serve_forever).start()

    def _callback(path, value):
        path = path[1:]
        if path in OSC.osc:
            if "vol" in path:
                OSC.osc[path] = int((value * 40) - 30)
            else:
                if value >= .5: OSC.osc[path] = 1
                else:           OSC.osc[path] = 0
            print(f"path: {path} | value: {OSC.osc[path]}")

class HIPBOX:

    def connect(connections):
        client = jack.Client("jack_connect_client_temp")
        client.activate()
        for c in connections: client.connect(c['input'],c['output'])
        return client

    # inp  = { "name": <name>, "port":  <port full>, "pan": <[R,L,C]> }
    # outp = { "name": <name>, "ports": [<port full>,..] }
    def __init__(self, inp, outp):
        self.input         = inp
        self.output        = outp
        self.pan           = inp['pan']
        self.var_name_base = f"{self.output['name']}_{self.input['name']}"

        for n in ["vol","solo","mute"]:
            var_name = f"{self.var_name_base}_{n}"
            OSC.osc[var_name] = 0

        self.client = jack.Client(self.var_name_base)
        self.client.set_process_callback(self._process_callback)
        self.client.set_shutdown_callback(self._shutdown_callback)

        self.inport    = self.client.inports.register(f"in_{self.input['name']}")
        self.outport_L = self.client.outports.register(f"out_{self.output['name']}_L")
        self.outport_R = self.client.outports.register(f"out_{self.output['name']}_R")

    def run(self):
        self.client.activate()
        self.client.connect(self.input['port'], f"{self.var_name_base}:in_{self.input['name']}")
        for i, port in enumerate(self.output['ports']):
            pan = "L" if i == 0 else "R"
            self.client.connect(f"{self.var_name_base}:out_{self.output['name']}_{pan}", port)
        return self

    def _process_callback(self, frames):
        if OSC.osc[f"{self.var_name_base}_mute"]:
            db = 0
        else:
            db = (10 ** (OSC.osc[f"{self.var_name_base}_vol"] / 20) )

        if self.input['pan'] in ['L','C']:
            self.outport_L.get_array()[:] = self.inport.get_array() * db
        if self.input['pan'] in ['R','C']:
            self.outport_R.get_array()[:] = self.inport.get_array() * db


    def _shutdown_callback(self, status, reason):
        print("JACK shutdown!")
        print("status:", status)
        print("reason:", reason)

if __name__ == "__main__":
    event = threading.Event()

    inputs = [
        { "name": "james",    "port": "james_fx:out_0",   "pan": "L" },
        { "name": "jesse",    "port": "jesse_fx:out_0",   "pan": "R" },
        { "name": "mitch",    "port": "mitch_fx:out_0",   "pan": "C" },
        { "name": "talkback", "port": "system:capture_4", "pan": "R" },
    ]
    outputs = [
        { "name": "mitch", "ports": ["system:playback_3", "system:playback_4"] },
        { "name": "james", "ports": ["system:playback_7", "system:playback_8"] },
        { "name": "jesse", "ports": ["system:playback_9", "system:playback_10"] },
    ]
    connections = [
        { "input": "system:capture_1", "output": "james_amp:in_0" },
        { "input": "system:capture_2", "output": "jesse_amp:in_0" },
        { "input": "system:capture_3", "output": "mitch_amp:in_0" },

        { "input": "james_amp:out_0", "output": "james_fx:in_0" },
        { "input": "jesse_amp:out_0", "output": "jesse_fx:in_0" },
        { "input": "mitch_amp:out_0", "output": "mitch_fx:in_0" },
    ]

    boxes = []
    osc_server = OSC()
    for outp in outputs:
        for inp in inputs:
            boxes.append(HIPBOX(inp, outp).run())
    boxes.append(HIPBOX.connect(connections))

    print("Press Ctrl+C to stop")
    try:
        event.wait()
    except KeyboardInterrupt:
        for box in boxes:
            box.client.deactivate()
            box.client.close()
        print("\nInterrupted by user")
