#!/usr/bin/env python3

from pythonosc import osc_server
from scipy.io import wavfile
import jack, threading, numpy

event = threading.Event()
# scale to -1.0 -- 1.0
nb_bits = 16 # -> 16-bit wav files
max_nb_bit = float(2 ** (nb_bits - 1))

def run_test():
    # aux = AUX("/home/mitch/hipbox/audio_files/blind2.wav")
    # aux.run()

    record = RECORD("/home/mitch/test.wav").run()

    print("Press Ctrl+C to stop")
    try:
        event.wait()
    except KeyboardInterrupt:
        print("\nInterrupted by user")

def run():
    inputs = [
        { "name": "james",    "port": "james_fx:out_0",   "pan": "L" },
        { "name": "jesse",    "port": "jesse_fx:out_0",   "pan": "R" },
        { "name": "mitch",    "port": "mitch_fx:out_0",   "pan": "C" },
        { "name": "talkback", "port": "system:capture_4", "pan": "C" },
    ]
    outputs = [
        { "name": "mitch", "ports": ["system:playback_3", "system:playback_4"] },
        { "name": "james", "ports": ["system:playback_7", "system:playback_8"] },
        { "name": "jesse", "ports": ["system:playback_9", "system:playback_10"] },
    ]
    connections = [
        # focusrite -> guitarix amps
        { "input": "system:capture_1", "output": "james_amp:in_0" },
        { "input": "system:capture_2", "output": "jesse_amp:in_0" },
        { "input": "system:capture_3", "output": "mitch_amp:in_0" },

        # guitarix amps -> guitarix fx/cabs
        { "input": "james_amp:out_0", "output": "james_fx:in_0" },
        { "input": "jesse_amp:out_0", "output": "jesse_fx:in_0" },
        { "input": "mitch_amp:out_0", "output": "mitch_fx:in_0" },
    ]
    additional_variables = [
        "talkback_toggle",
        "talkback_talkback_solo",
    ]

    boxes = []
    osc_server = OSC(additional=additional_variables)
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


class OSC:
    osc = {}

    def __init__(self, grep="/*", port=3001, additional=[]):
        from pythonosc import dispatcher

        for a in additional:
            OSC.osc[a] = 0

        disp = dispatcher.Dispatcher()
        disp.map(grep, self._callback)
        server = osc_server.ThreadingOSCUDPServer(('0.0.0.0', port), disp)
        threading.Thread(target=server.serve_forever).start()

    def _callback(self, path, value):
        path = path[1:]
        if path in OSC.osc:
            if "vol" in path:
                OSC.osc[path] = int((value * 40) - 30)
            elif "talkback_toggle" == path:
                for key in OSC.osc:
                    if "talkback" in key and not "vol" in key:
                        if value >= .5:
                            OSC.osc[key] = 1
                        else:
                            OSC.osc[key] = 0
            else:
                if value >= .5: OSC.osc[path] = 1
                else:           OSC.osc[path] = 0
            # print(f"----> path: {path} | value: {OSC.osc[path]}")

class AUX:
    # outp = { "name": <name>, "port": <port full> }
    def __init__(self, audio_file):
        self.client         = jack.Client("AUX")
        self.play_head      = 0
        fs, self.audio_file = wavfile.read(audio_file)

        self.audio_file = self.audio_file / (max_nb_bit + 1.0)

        self.client.set_process_callback(self._process_callback)
        self.client.set_shutdown_callback(self._shutdown_callback)

        self.outport = self.client.outports.register("out_0")

    def run(self):
        self.client.activate()
        return self

    def _process_callback(self, frames):
        bs = self.client.blocksize
        db = (10 ** (-10 / 20) )

        self.outport.get_array()[:] = self.audio_file[self.play_head:self.play_head+bs] * db
        self.play_head += bs
        if self.audio_file.size <= self.play_head:
            self.client.deactivate()
            self.client.close()

    def _shutdown_callback(self, status, reason):
        print("JACK shutdown!")
        print("status:", status)
        print("reason:", reason)
        
class RECORD:

    # save: https://docs.scipy.org/doc/scipy/reference/generated/scipy.io.wavfile.write.html

    # outp = { "name": <name>, "port": <port full> }
    def __init__(self, filename):
        self.client     = jack.Client("RECORD")
        self.audio_file = None
        self.filename   = filename

        self.client.set_process_callback(self._process_callback)
        self.client.set_shutdown_callback(self._shutdown_callback)

        self.inport = self.client.inports.register("in_0")

    def run(self):
        self.client.activate()
        self.client.connect("system:capture_4","RECORD:in_0")
        return self

    def _process_callback(self, frames):
        bs = self.client.blocksize
        db = (10 ** (-10 / 20) )

        if self.audio_file is None:
            self.audio_file = self.inport.get_array() * (max_nb_bit + 1.0)
        else:
            self.audio_file = numpy.concatenate((self.audio_file, self.inport.get_array() * (max_nb_bit + 1.0)))

        print(self.audio_file.size)

    def _shutdown_callback(self, status, reason):
        print("JACK shutdown!")
        print("status:", status)
        print("reason:", reason)


class HIPBOX:
    # connections = [{"input":"<port full>", "output": "<port full>"},..]
    def connect(connections):
        client = jack.Client("jack_connect_client_temp")
        client.activate()
        for c in connections: client.connect(c['input'],c['output'])
        return client

    # inp  = { "name": <name>, "port":  <port full>, "pan": <[R,L,C]> }
    # outp = { "name": <name>, "ports": [<port full>,..] }
    def __init__(self, inp, outp):
        self.input              = inp
        self.output             = outp
        self.pan                = inp['pan']
        self.var_name_base      = f"{self.output['name']}_{self.input['name']}"
        self.solo_var_name_base = f"{self.input['name']}_{self.input['name']}"

        self.solo_var_name      = f"{self.solo_var_name_base}_solo"
        self.mute_var_name      = f"{self.var_name_base}_mute"

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
        db = 0
        gain = OSC.osc[f"{self.var_name_base}_vol"]

        if self.mute_var_name in OSC.osc and self.solo_var_name in OSC.osc:
            if not OSC.osc[self.mute_var_name] and not OSC.osc[self.solo_var_name]:
                db = (10 ** (gain / 20) )

        if self.input['pan'] in ['L','C']:
            self.outport_L.get_array()[:] = self.inport.get_array() * db
        if self.input['pan'] in ['R','C']:
            self.outport_R.get_array()[:] = self.inport.get_array() * db

    def _shutdown_callback(self, status, reason):
        print("JACK shutdown!")
        print("status:", status)
        print("reason:", reason)


if __name__ == "__main__":
    run()

