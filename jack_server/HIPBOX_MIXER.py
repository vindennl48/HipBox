from OSC_SERVER import OSC
import jack

class HIPBOX_MIXER:
    # inports = [ {"name": <port name>, "osc_vol": <osc value for vol>,
    #     "osc_mute": <..for mute>, "osc_solo": <..for solo>, "pan": <L,R,C>} ]
    # outport = { "name": <port name>, "L": <system port name L>, "R": <system port name R> }
    def __init__(self, name, inports, outport):
        self.name    = f"{name}_HBM"
        self.inports = inports
        self.outport = outport
        self.client  = jack.Client(self.name)

        for i, inp in enumerate(self.inports):
            self.inports[i]["port_obj"] = self.client.inports.register(inp["name"])
        self.outport["port_objs"] = {}
        self.outport["port_objs"]["L"] = self.client.outports.register(f"{self.outport['name']}_L")
        self.outport["port_objs"]["R"] = self.client.outports.register(f"{self.outport['name']}_R")

        self.client.set_process_callback( self._process_callback )
        self.client.set_shutdown_callback( self._shutdown_callback )

    def run(self):
        self.client.activate()
        self.client.connect(f"{self.name}:{self.outport['name']}_L", self.outport["L"])
        self.client.connect(f"{self.name}:{self.outport['name']}_R", self.outport["R"])
        return self

    def kill(self):
        self.client.deactivate()
        self.client.close()

    def _process_callback(self, frames):
        audio_L = None
        audio_R = None
        for inp in self.inports:
            if (inp["osc_mute"] is not None and OSC.get(inp["osc_mute"])) \
                or (inp["osc_solo"] is not None and OSC.get(inp["osc_solo"])):
                gain = 0
            else:
                gain =  ( 10 ** (OSC.get(inp["osc_vol"]) / 20) )
            # if inp["osc_vol"] == "jesse_talkback_vol":
            print(f"gain: {gain} | osc_vol: {inp['osc_vol']}")
            if inp["pan"] in ["L","C"]:
                if audio_L is None:
                    audio_L = inp["port_obj"].get_array() * gain
                else:
                    audio_L += inp["port_obj"].get_array() * gain
            if inp["pan"] in ["R","C"]:
                if audio_R is None:
                    audio_R = inp["port_obj"].get_array() * gain
                else:
                    audio_R += inp["port_obj"].get_array() * gain
        self.outport["port_objs"]["L"].get_array()[:] = audio_L
        self.outport["port_objs"]["R"].get_array()[:] = audio_R

    def _shutdown_callback(self, status, reason):
        print("JACK shutdown!")
        print("status:", status)
        print("reason:", reason)



class HIPBOX_VOLUME:
    def __init__(self, name, osc_vol, osc_mute=None, osc_solo=None):
        self.osc_vol     = osc_vol
        self.osc_mute    = osc_mute
        self.osc_solo    = osc_solo
        self.client_name = f"{name}_HB"
        self.client      = jack.Client(self.client_name)
        self.inport      = self.client.inports.register(f"in_0")
        self.outport     = self.client.outports.register(f"out_0")

        self.client.set_process_callback( self._process_callback )
        self.client.set_shutdown_callback( self._shutdown_callback )

    def run(self):
        self.client.activate()
        return self

    def _process_callback(self, frames):
        if (self.osc_mute is not None and OSC.get(self.osc_mute)) \
            or (self.osc_solo is not None and OSC.get(self.osc_solo)):
            gain = 0
        else:
            gain = OSC.get(self.osc_vol)
        self.outport.get_array()[:] = self.inport.get_array() * ( 10 ** (gain / 20) )

    def _shutdown_callback(self, status, reason):
        print("JACK shutdown!")
        print("status:", status)
        print("reason:", reason)

    def kill(self):
        self.client.deactivate()
        self.client.close()


class HIPBOX_CONNECT:
    def __init__(self, connections):
        self.connections = {}
        self.client      = jack.Client("HIPBOX_CONNECT")

        if type(connections) is list:
            for c in connections:
                try:
                    self.client.connect(c['inport'],c['outport'])
                except:
                    print("Connection already exists or is impossible!!")
        else:
            try:
                self.client.connect(connections['inport'],connections['outport'])
            except:
                print("Connection already exists or is impossible!!")

    def run(self):
        self.client.activate()
        return self

    def kill(self):
        self.client.deactivate()
        self.client.close()
