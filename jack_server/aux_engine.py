from scipy.io import wavfile
import jack, threading, numpy, time


# scale to -1.0 -- 1.0
nb_bits = 16 # -> 16-bit wav files
max_nb_bit = float(2 ** (nb_bits - 1))


def run():
    event = threading.Event()

    aux = AuxEngine()
    aux.load_audio("blind",{ "filepath":"/home/mitch/hipbox/audio_files/blind2.wav", "start":0, "end":2646000 })

    aux.run()
    print("aux ran")

    time.sleep(5)
    print("slept 5sec")

    aux.play("blind")
    print("played blind")

    time.sleep(5)
    print("slept 5sec")

    aux.play("blind")
    print("played blind")

    time.sleep(5)
    print("slept 5sec")

    aux.stop()
    print("stopped")

    print("Press Ctrl+C to stop")
    try:
        event.wait()
    except KeyboardInterrupt:
        print("\nInterrupted by user")


class AudioClip:
    def __init__(self, name, filepath, blocksize, start, end=None, loop=False):
        self.name       = name
        self.filepath   = filepath
        self.start      = start
        self.end        = end
        self.data       = None
        self.playhead   = start
        self.is_playing = False
        self.blocksize  = blocksize
        self.loop       = loop

        fs, audio       = wavfile.read(filepath)
        self.data       = audio / (max_nb_bit + 1.0)

        if self.end is None: self.end = len(self.data)

    def get_next_block():
        result = numpy.zeros(self.blocksize)
        if self.is_playing:
            if self.playhead >= self.end:
                if self.loop:
                    self.playhead = self.start
                else:
                    self.is_playing = False
                    self.playhead   = self.start
            else:
                result[:] = self.data[self.playhead:self.playhead+self.blocksize]
                self.playhead += self.blocksize
        return result

    def play(self):
        self.is_playing = True

    def stop(self):
        self.is_playing = False
        self.playhead   = self.start

    def pause(self):
        self.is_playing = False



class AuxEngine:
    # outp = { "name": <name>, "port": <port full> }
    def __init__(self):
        self.client              = jack.Client("AUX")
        self.blocksize           = self.client.blocksize
        self.outport             = self.client.outports.register("out_0")
        self.bpm                 = 120
        self.timesig             = 4

        self.sample_rate         = 44100
        self.samples_per_beat    = None
        self.samples_per_measure = None
        self.playhead            = 0

        self.is_playing            = False
        self.audiofile_next_target = None
        self.audiofile_target      = None
        self.audiofiles            = {}
        self.audiofile_playhead    = None

        self.client.set_process_callback(self._process_callback)
        self.client.set_shutdown_callback(self._shutdown_callback)

        self.set_samples_per_beat()
        self.set_samples_per_measure()


    def run(self):
        self.client.activate()
        self.client.connect("AUX:out_0", "system:playback_7")
        self.client.connect("AUX:out_0", "system:playback_8")
        return self


    def _process_callback(self, frames):
        audio_out = numpy.zeros(self.blocksize)

        if self.advance_playhead():
            if self.audiofile_next_target is not None:
                self.audiofile_target      = self.audiofile_next_target
                self.audiofile_next_target = None
                self.audiofile_playhead    = self.audiofiles[self.audiofile_target]["start"]
                self.is_playing            = True
                print(f"Playing song: {self.audiofile_target}")

        if self.is_playing:
            if self.audiofile_playhead >= self.audiofiles[self.audiofile_target]["end"]:
                self.is_playing = False
            else:
                audio_out += self.audiofiles[
                    self.audiofile_target]["data"][
                    self.audiofile_playhead:self.audiofile_playhead+self.blocksize]
                self.audiofile_playhead += self.blocksize

        else:
            self.audiofile_playhead = None
            self.audiofile_target   = None

        self.outport.get_array()[:] = audio_out

    
    def advance_playhead(self):
        self.playhead += self.blocksize
        if self.playhead >= self.samples_per_measure:
            self.playhead = self.playhead - self.samples_per_measure
            return True
        return False


    def _shutdown_callback(self, status, reason):
        print("JACK shutdown!")
        print("status:", status)
        print("reason:", reason)


    def play(self, name):
        self.audiofile_next_target = name


    def stop(self):
        self.audiofile_next_target = None
        self.is_playing            = False


    # name: "Blind" { "filepath":"../audio_files/blind.wav", "start":43256, "end":2395453 }
    def load_audio(self, name, audiofile_object):
        self.audiofiles[name]         = audiofile_object
        fs, audio                     = wavfile.read(audiofile_object["filepath"])
        self.audiofiles[name]["data"] = audio / (max_nb_bit + 1.0)

    def set_bpm(self, bpm):
        self.bpm = bpm
        self.set_samples_per_beat()
        self.set_samples_per_measure()

    def set_time_sig(self, timesig):
        self.timesig = timesig
        self.set_samples_per_measure()

    def set_samples_per_beat(self):
        self.samples_per_beat = (60/self.bpm)*self.sample_rate

    def set_samples_per_measure(self):
        self.samples_per_measure = self.samples_per_beat * self.timesig


if __name__ == "__main__":
    run()
