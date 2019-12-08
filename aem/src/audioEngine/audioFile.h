#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include <sndfile.h>
#include <string>
#include <vector>
#include "../extra/print.h"

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
      PRINT("audioEngine.load_audio_file> Cannot open file for reading: %s", filename.c_str());
      return 0;
    }

    if (!info.frames) {
      PRINT("audioEngine.load_audio_file> File is empty: %s", filename.c_str());
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

#endif
