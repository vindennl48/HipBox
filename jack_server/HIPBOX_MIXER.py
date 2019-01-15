from OSC_SERVER import OSC
import jack

class HIPBOX_VOLUME:
    def __init__(self, name, osc_vol, osc_mute=None, osc_solo=None):
        self.inport      = self.client.inports.register(f"in_0")
        self.outport     = self.client.outports.register(f"out_0")
        self.osc_vol     = osc_vol
        self.osc_mute    = osc_mute
        self.osc_solo    = osc_solo
        self.client_name = f"{name}_HB"
        self.client      = jack.Client(self.client_name)

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
                self.client.connect(c['inport'],c['outport'])
        else:
            self.client.connect(connections['inport'],connections['outport'])

    def run(self):
        self.client.activate()
        return self

    def kill(self):
        self.client.deactivate()
        self.client.close()
