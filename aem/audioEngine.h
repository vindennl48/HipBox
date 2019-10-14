#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <sndfile.h>
#include <string>
#include <vector>

using namespace std;

struct AudioFile {
  string        name      = "";
  string        filename  = "";
  unsigned int  start_pos = 0;
  unsigned int  end_pos   = 0;
  long long     pos       = 0;
  bool          is_repeat = false;
  SNDFILE       *in       = NULL;
  SF_INFO       info;
  vector<float> buffer;

  int load_audio_file() {
    in = sf_open(filename.c_str(), SFM_READ, &info);
    if (!in) {
      PRINTD("\n\n----> SNDFILE> Cannot open file for reading: %s\n\n", filename.c_str());
      return 0;
    }

    if (!info.frames) {
      PRINTD("\n\n----> SNDFILE> File is empty: %s\n\n", filename.c_str());
      return 0;
    }

    if (!end_pos || end_pos > info.frames)
      end_pos = info.frames;
    if (start_pos >= end_pos)
      start_pos = 0;

    float *sample;
    for (int i=0; i<end_pos; i++) {
      sf_readf_float(in, sample, 1);
      if (i >= start_pos)
        buffer.push_back(*sample);
    }

    sf_close(in);
    return 1;
  }

  float* get_next_sample() {
    if (pos < 0)
      return NULL;

    float *result = &buffer[pos];

    if (++pos >= end_pos) {
      if (is_repeat)
        pos = 0;
      else
        pos = -1;
    }

    return result;
  }

  void reset() { pos = 0; }
};

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
