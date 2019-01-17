from scipy.io import wavfile
import jack, threading, numpy

class RECORD:

    # save: https://docs.scipy.org/doc/scipy/reference/generated/scipy.io.wavfile.write.html

    # inports  = [{"name": <port_name>, "pan": <pan = L,R,C>]},..]
    # mix      = [<port to be in stereo mix>,..]
    # save_dir = "/home/user/saveaudio"
    def __init__(self, inports, mix, save_dir):
        self.client      = jack.Client("RECORD")
        self.max_nb_bit  = float(2 ** (16 - 1))
        self.master_L    = []
        self.master_R    = []
        self.audio_files = {}
        self.inports     = inports

        self.client.set_process_callback(self._process_callback)
        self.client.set_shutdown_callback(self._shutdown_callback)

        for i, inp in enumerate(self.inports):
            self.inports[i]["port"] = self.client.inports.register(inp["name"])
            self.audio_files[inp["name"]] = []

    def run(self):
        self.client.activate()
        return self

    def _process_callback(self, frames):
        audio_L = None
        audio_R = None

        for inp in self.inports:
            array = inp["port"].get_array()
            name  = inp["name"]
            pan   = inp["pan"]

            self.audio_files[name].append(array)
            if pan in ["L","C"]: audio_L = array if audio_L is None else audio_L + array
            if pan in ["R","C"]: audio_R = array if audio_R is None else audio_R + array

        self.master_L.append(audio_L)
        self.master_R.append(audio_R)

        print(f"length: {len(self.master_L)}")

    def _shutdown_callback(self, status, reason):
        print("JACK shutdown!")
        print("status:", status)
        print("reason:", reason)

    # static
    def convert_to_int_array(array):
        return array * (self.max_nb_bit + 1.0)
