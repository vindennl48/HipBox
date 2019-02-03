from pythonosc import udp_client
import jack, threading, struct, mido


class MidiEngine:
    def __init__(self, connections, midi_map, osc_return_port=4000, ip='127.0.0.1'):
        self.name        = f"MIDI_TO_OSC"
        self.client      = jack.Client(self.name)
        self.inport      = self.client.midi_inports.register('in_0')
        self.midi_map    = midi_map
        self.osc_client  = udp_client.SimpleUDPClient(ip, osc_return_port)
        self.connections = connections

        self.client.set_process_callback(self._process_callback)

    def run(self):
        self.client.activate()
        for connection in self.connections:
            self.client.connect(connection, "MIDI_TO_OSC:in_0")
        return self

    def _process_callback(self, frames):
        for offset, data in self.inport.incoming_midi_events():
            try:
                status, note, vel = struct.unpack('3B', data)
            except:
                return 0

            if status == 176:
                ntype = "cc"
            elif status == 144:
                ntype = "note"
            else:
                ntype = None


            if ntype is not None:
                print(f"----> MIDI: type: {ntype}, note: {note}, vel: {vel}")
                if f"{ntype}_{note}" in self.midi_map:
                    responses = self.midi_map[f"{ntype}_{note}"]
                    for response in responses:
                        path = response

                        if type(response) == list:
                            path = response[0]
                            vel  = response[1:]

                        self.osc_client.send_message(path, vel)
