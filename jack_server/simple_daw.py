from scipy.io import wavfile
import jack, threading, numpy, time


class SimpleDAW:
    def __init__(self, bitrate=16, sample_rate=44100):
        self.client              = jack.Client("SimpleDAW")
        self.blocksize           = self.client.blocksize
        self.outport             = self.client.outports.register("out_0")
        self.bpm                 = 120
        self.timesig             = 4
        self.bitrate             = bitrate

        self.sample_rate         = sample_rate
        self.samples_per_beat    = None
        self.samples_per_measure = None
        self.measure             = 0            # counter
        self.beat                = 0            # counter
        self.click_on            = False
        self.click_outport       = self.client.outports.register("out_click")

        self.queue               = []
        self.clips               = {}

        self.client.set_process_callback(self._process_callback)
        self.client.set_shutdown_callback(self._shutdown_callback)

        self.set_samples_per_beat()
        self.set_samples_per_measure()


    def run(self):
        if "click_high" not in self.clips \
            or "click_low" not in self.clips:
            raise Exception("####> No click clips found!")

        else:
            self.client.activate()

        return self


    def load_audio(self, name, filepath, start, end=None, loop=False):
        self.clips[name] = AudioClip(name, filepath, self.blocksize, start, end, loop, self.bitrate)


    def _process_callback(self, frames):
        audio_out = numpy.zeros(self.blocksize)
        click_out = numpy.zeros(self.blocksize)

        if self.advance_measure():
            # click high for click beat 1
            self.advance_beat()
            if self.click_on:
                self.clips["click_high"].play()

            while self.queue:
                name = self.queue.pop()
                self.clips[name].play()

        elif self.advance_beat():
            # click low for click other beats
            if self.click_on:
                self.clips["click_low"].play()

        for name in self.clips:
            if self.clips[name].is_playing:
                if "click" in name:
                    click_out += self.clips[name].get_next_block()
                else:
                    audio_out += self.clips[name].get_next_block()

        self.outport.get_array()[:]       = audio_out
        self.click_outport.get_array()[:] = click_out


    def _shutdown_callback(self, status, reason):
        print("JACK shutdown!")
        print("status:", status)
        print("reason:", reason)


    def process_osc(self, path, value):
        path = path[1:]
        path_split = path.split("_")

        if path_split[0] == "click":
            if path_split[1] == "toggle":
                self.click_on = False if self.click_on else True
            elif path_split[1] == "on":
                self.click_on = True
            elif path_split[1] == "off":
                self.click_on = False

        elif path_split[0] == "play":
            self.play(path_split[1])

        elif path_split[0] == "stop":
            if path_split[1] == "all":
                self.stop_all()
            else:
                self.stop(path_split[1])

        elif path_split[0] == "timesig":
            self.set_time_sig(int(path_split[1]))

        elif path_split[0] == "bpm":
            self.set_bpm(int(path_split[1]))


    def advance_measure(self):
        self.measure += self.blocksize
        if self.measure >= self.samples_per_measure:
            self.measure = self.measure - self.samples_per_measure
            return True
        return False


    def advance_beat(self):
        self.beat += self.blocksize
        if self.beat >= self.samples_per_beat:
            self.beat = self.beat - self.samples_per_beat
            # print("click")
            return True
        return False


    def stop(self, name):
        if name in self.clips:
            self.clips[name].stop()


    def stop_all(self):
        for name in self.clips:
            self.clips[name].stop()


    def play(self, name):
        if name in self.clips:
            self.queue.append(name)


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


class AudioClip:
    def __init__(self, name, filepath, blocksize, start, end=None, loop=False, bitrate=16):
        self.name       = name
        self.filepath   = filepath
        self.start      = start
        self.end        = end
        self.data       = None
        self.playhead   = start
        self.is_playing = False
        self.is_reset   = False
        self.blocksize  = blocksize
        self.loop       = loop
        self.max_nb_bit = float(2 ** (bitrate - 1))

        fs, audio       = wavfile.read(filepath)
        self.data       = audio / (self.max_nb_bit + 1.0)

        if self.end is None: self.end = len(self.data)

    def get_next_block(self):
        audio_out = numpy.zeros(self.blocksize)
        if self.is_playing:
            if self.is_reset:
                self.is_reset = False
                self.playhead = self.start

            if self.playhead+self.blocksize >= self.end and self.loop:
                self.playhead = self.start

            if self.playhead+self.blocksize >= self.end and not self.loop:
                self.is_playing = False
                self.playhead   = self.start
                return audio_out

            audio_out[:] = self.data[self.playhead:self.playhead+self.blocksize]
            self.playhead += self.blocksize

        return audio_out

    def play(self):
        if self.is_playing:
            self.is_reset = True
        else:
            self.is_playing = True

    def stop(self):
        self.is_playing = False
        self.playhead   = self.start

    def pause(self):
        self.is_playing = False



