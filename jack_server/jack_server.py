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
                HIPBOX.gvars[f"{o['hb']}_{i['hb']}"] = 0

        # OSC SERVER
        ############
        def _callback(path, value):
            # print(f"path: {path}")
            outp, inp, kind = tuple(path[1:].split('_'))
            if kind == "vol":
                name = f"{outp}_{inp}"

                if name in HIPBOX.gvars:
                    db = int((value * 40) - 30)
                    HIPBOX.gvars[name] = db
                    print(f"path: {path} | {name}: {db}")

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
            destination[:] = numpy.add(destination, audio)
        return destination

    def _process_callback(frames):
        for outp_key in HIPBOX.outputs:
            output_array_L = None
            output_array_R = None
            outp           = HIPBOX.outputs[outp_key]
            outp_name      = outp_key.split('_')[1]

            for i, inp_key in enumerate(HIPBOX.inputs):
                inp_name = inp_key.split('_')[1]
                db       = (10 ** (HIPBOX.gvars[f"{outp_name}_{inp_name}"] / 20) )
                inp      = HIPBOX.inputs[inp_key]

                for port in outp['port']:
                    if inp['pan'] == 'L':
                        output_array_L = HIPBOX.add_audio(output_array_L, (inp['port'].get_array()*db) )
                    elif inp['pan'] == 'R':
                        output_array_R = HIPBOX.add_audio(output_array_R, (inp['port'].get_array()*db) )
                    elif inp['pan'] == 'C':
                        output_array_L = HIPBOX.add_audio(output_array_L, (inp['port'].get_array()*db) )
                        output_array_R = HIPBOX.add_audio(output_array_R, (inp['port'].get_array()*db) )

            outp['port'][0].get_array()[:] = output_array_L
            outp['port'][1].get_array()[:] = output_array_R

    def _shutdown_callback(status, reason):
        print("JACK shutdown!")
        print("status:", status)
        print("reason:", reason)
        HIPBOX.event.set()

HIPBOX.run(inputs, outputs, extra_connections)
