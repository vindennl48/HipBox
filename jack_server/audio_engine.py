from scipy.io import wavfile
import jack, threading, numpy, time


def run():
    event = threading.Event()

    aux = AudioEngine(connections=["system:playback_7","system:playback_8"])
    aux.load_audio(
        name     = "blind",
        filepath = "/home/mitch/hipbox/audio_files/blind2.wav",
        start    = 0,
    )

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

    def get_next_block():
        audio_out = numpy.zeros(self.blocksize)
        if self.is_playing:
            if self.playhead >= self.end and not self.loop:
                self.is_playing = False
                self.playhead   = self.start
                return audio_out

            if (self.playhead >= self.end and self.loop) \
                or self.is_reset:
                self.is_reset = False
                self.playhead = self.start

            audio_out[:] = self.data[self.playhead:self.playhead+self.blocksize]
            self.playhead += self.blocksize

        return audio_out

    def play(self):
        if self.is_playing:
            self.is_reset = False
        else:
            self.is_playing = True

    def stop(self):
        self.is_playing = False
        self.playhead   = self.start

    def pause(self):
        self.is_playing = False



class AudioEngine:
    def __init__(self, connections, bitrate=16, sample_rate=44100):
        self.client              = jack.Client("AUX")
        self.blocksize           = self.client.blocksize
        self.outport             = self.client.outports.register("out_0")
        self.bpm                 = 120
        self.timesig             = 4
        self.connections         = connections
        self.bitrate             = bitrate

        self.sample_rate         = sample_rate
        self.samples_per_beat    = None
        self.samples_per_measure = None
        self.measure             = 0
        self.beat                = 0
        self.metronome_on        = False

        self.queue               = []
        self.clips               = {}

        self.client.set_process_callback(self._process_callback)
        self.client.set_shutdown_callback(self._shutdown_callback)

        self.set_samples_per_beat()
        self.set_samples_per_measure()


    def run(self):
        if "click_high" not in self.clips \
            or "click_low" not in self.clips:
            raise Exception("####> No metronome clips found!")

        else:
            self.client.activate()
            for connection in connections:
                self.client.connect("AUX:out_0", connection)

        return self


    def load_audio(self, name, filepath, start, end=None, loop=False):
        self.clips[name] = AudioClip(name, filepath, self.blocksize, start, end, loop, self.bitrate)


    def _process_callback(self, frames):
        audio_out = numpy.zeros(self.blocksize)

        if self.advance_measure():
            # click high for metronome beat 1
            self.advance_beat()
            if self.metronome_on:
                clips["click_high"].play()

            while self.queue:
                name = self.queue.pop()
                clips[name].play()

        elif self.advance_beat():
            # click low for metronome other beats
            if self.metronome_on:
                clips["click_low"].play()

        for name in clips:
            if clips[name].is_playing:
                audio_out += clips[name].get_next_block()

        self.outport.get_array()[:] = audio_out


    def _shutdown_callback(self, status, reason):
        print("JACK shutdown!")
        print("status:", status)
        print("reason:", reason)


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
            return True
        return False


    def stop(self, name):
        if name in clips:
            clips[name].stop()


    def stop_all(self):
        for name in clips:
            clips[name].stop()


    def play(self, name):
        if name in clips:
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


#
#    def _process_callback(self, frames):
#        audio_out = numpy.zeros(self.blocksize)
#
#        if self.advance_playhead():
#            if self.audiofile_next_target is not None:
#                self.audiofile_target      = self.audiofile_next_target
#                self.audiofile_next_target = None
#                self.audiofile_playhead    = self.clips[self.audiofile_target]["start"]
#                self.is_playing            = True
#                print(f"Playing song: {self.audiofile_target}")
#
#        if self.is_playing:
#            if self.audiofile_playhead >= self.clips[self.audiofile_target]["end"]:
#                self.is_playing = False
#            else:
#                audio_out += self.clips[
#                    self.audiofile_target]["data"][
#                    self.audiofile_playhead:self.audiofile_playhead+self.blocksize]
#                self.audiofile_playhead += self.blocksize
#
#        else:
#            self.audiofile_playhead = None
#            self.audiofile_target   = None
#
#        self.outport.get_array()[:] = audio_out
#
#    


if __name__ == "__main__":
    run()
