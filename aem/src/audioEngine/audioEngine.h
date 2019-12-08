#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <string>
#include <vector>
#include "audioFile.h"

using namespace std;

struct AudioEngine {
  vector<AudioFile> backtracks;
  AudioFile         high_click;
  AudioFile         low_click;
  unsigned int      time_sig               = 4;
  unsigned int      bpm                    = 120;
  unsigned int      beat_pos               = 0;       // (44100*60)/bpm
  unsigned int      measure_pos            = 0;       // ((44100*60)/bpm)*time_sig
  float             sample                 = 0.0;
  bool              is_high_click          = true;
  bool              is_click_playing       = false;   // is click audio being sent
  bool              are_backtracks_playing = false;   // is backtrack audio being sent
  bool              is_playing             = false;   // is the AudioEngine running

  unsigned int get_max_beat_frames()    { return 264600/bpm; }
  unsigned int get_max_measure_frames() { return get_max_beat_frames()*time_sig; }

  float* get_next_click_sample() {
    float *sample_p;

    if (!beat_pos) {
      high_click.reset();
      low_click.reset();
    }

    if (is_high_click && measure_pos < get_max_beat_frames())
      sample_p = high_click.get_next_sample();
    else
      sample_p = low_click.get_next_sample();

    if (!is_click_playing)
      sample_p = NULL;

    if (++beat_pos    >= get_max_beat_frames())    beat_pos    = 0;
    if (++measure_pos >= get_max_measure_frames()) measure_pos = 0;

    return sample_p;
  }

  float* get_next_sample() {
    float *sample_p;

    sample_p = get_next_click_sample();
    if (sample_p)
      sample = *sample_p;
    else
      sample = 0.0;

    if (are_backtracks_playing) {
      int i_size = backtracks.size();
      for (int i=0; i<i_size; i++) {
        sample_p = backtracks[i].get_next_sample();
        if (sample_p)
          sample += *sample_p;
      }
    }

    return &sample;
  }

  void reset() {
    high_click.reset();
    low_click.reset();

    beat_pos    = 0;
    measure_pos = 0;

    int i_size = backtracks.size();
    for (int i=0; i<i_size; i++)
      backtracks[i].reset();
  }
};

#endif
