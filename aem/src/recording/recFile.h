#ifndef RECFILE_H
#define RECFILE_H

#include <unistd.h>
#include <vector>
#include <string>
#include <sndfile.h>
#include "../routing/inPort.h"
#include "../extra/isDir.h"
#include "../extra/print.h"

#define RING_BUFF_SIZE (44100 * 10)

using namespace std;

static time_t now  = time(0);
static tm     *ltm = localtime(&now);

struct RecFile {
  static unsigned int recNum;

  vector<InPort*> in_ports_p;
  SF_INFO         info;
  SNDFILE         *out      = NULL;
  unsigned int    id        = 0;
  string          base_name = "";
  string          filename  = "";

  // ring-buffer
  vector<float> ring_buff      = vector<float>(RING_BUFF_SIZE);
  unsigned int  ring_buff_size = RING_BUFF_SIZE;
  bool          wrap           = false;
  unsigned int  buff_pos       = 0;
  unsigned int  write_pos      = 0;

  RecFile() {
    info.samplerate = 44100;
    info.channels   = 1;
    info.format     = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
  }

  ~RecFile() {
    if (out) sf_close(out);
    ring_buff.clear();
  }

  void set_next_jack_buff(jack_nframes_t nframes) {
    float sample = 0.0;
    int i_size   = 0;

    for (int n=0; n<nframes; n++) {
      sample = 0.0;

      i_size = in_ports_p.size();
      for (int i=0; i<i_size; i++)
        sample += in_ports_p[i]->input[n];

      set_next_sample(sample);
    }
  }

  void set_next_sample(float sample) {
    ring_buff[buff_pos] = sample;

    if (++buff_pos >= ring_buff_size) {
      buff_pos = 0;
      wrap     = true;
    }
    if (wrap) {
      if (buff_pos >= write_pos) {
        if (buff_pos + 1 >= ring_buff_size) {
          write_pos = 0;
          wrap      = false;
        } else {
          write_pos = buff_pos + 1;
        }
      }
    }
  }

  float* get_next_sample() {
    while (write_pos == buff_pos) usleep(10);

    float *result = &ring_buff[write_pos];

    if (++write_pos >= ring_buff_size) {
      write_pos = 0;
      wrap      = false;
    }

    return result;
  }

  void write_next_sample() {
    sf_writef_float(out, get_next_sample(), 1);
  }

  void create_file() {
    char num_of_rec[3];
    char month[3];
    char day[3];
    sprintf(num_of_rec, "%02d", recNum);
    sprintf(month,      "%02d", 1+ltm->tm_mon);
    sprintf(day,        "%02d", ltm->tm_mday);
    string num_of_rec_s = string(num_of_rec);

    filename  = "recordings/" + num_of_rec_s + "/";
    filename += to_string(1900+ltm->tm_year) + string(month) + string(day);
    filename += "_" + base_name + "_" + num_of_rec_s + ".wav";

    if (!isDir("recordings/"+num_of_rec_s)) {
      PRINT("recording.create_file> 'recordings/%s' DIR doesn't exist.. Creating Directory", num_of_rec_s.c_str());
      string cmd = "mkdir -p recordings/"+num_of_rec_s;
      system(cmd.c_str());
    }

    // Open the wav file for writing
    out = sf_open(filename.c_str(), SFM_WRITE, &info);
    if (!out) {
      PRINT("recording.create_file> SNDFILE> Cannot open file for writing");
    }
  }

  void close_file() { sf_close(out); }

};

#endif
