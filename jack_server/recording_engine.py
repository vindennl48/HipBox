from pythonosc import udp_client
import os

class RecordingEngine:
    # inputs = { "name": {"port": "<audio_port>", "pan": "<L,R,C>", "record": <True,False>} }
    def __init__(self, inputs,
            filename_prepend = "recorded/scratch_",
            mixed_rec_ports  = ["record_mix:out_left","record_mix:out_right"],
            osc_start_port   = 4000,
            osc_ip           = '127.0.0.1'):

        self.inputs           = inputs
        self.clients          = []
        self.mixed_rec_ports  = mixed_rec_ports
        self.filename_prepend = filename_prepend
        self.start_port       = osc_start_port
        self.ip               = osc_ip

    def start_record(self):
        self.start_channel_record()
        self.start_mixed_record()

    def start_channel_record(self):
        if not self.clients:
            port_offset = 1
            for name in self.inputs:
                if self.inputs[name]["record"]:
                    port   = self.start_port + port_offset
                    inport = self.inputs[name]["port"]
                    os.system(f"jack_capture -ns -b 16 -c 1 --osc {port} -mp3 -p {inport} -fp recorded/{name}_ &")
                    self.clients.append( udp_client.SimpleUDPClient(self.ip, self.start_port) )
                    port_offset += 1
        # else:
            # print("####> You already have recorders open!")

    def start_mixed_record(self):
        port     = self.start_port
        outports = self.mixed_rec_ports
        prepend  = self.filename_prepend
        os.system(f"jack_capture -ns -b 16 -c 2 --osc {port} -mp3 -p {outports[0]} -p {outports[1]} -fp {filename_prepend} &")

    def stop_recording(self):
        for client in self.clients:
            client.send_message("/jack_capture/stop", [])
        self.clients.clear()

    def process_osc(self, path, value):
        path_sp = path[1:].split('/')

        # This only processes osc that are prefaced by 'mixer'
        if path_sp[0] != "record": return 0

        # remove 'mixer' from path
        path_sp.pop(0)

        # handle different length paths
        if len(path_sp) == 1:
            # only kind right now is stop
            kind = path_sp[0]
        else:
            kind = "start"

        if kind == "start":
            self.start_record()
        elif kind == "stop":
            self.stop_recording()
        else:
            print("####> Error processing osc in recorder")

        return 1
