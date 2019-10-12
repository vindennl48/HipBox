#ifndef STRUCTS_H
#define STRUCTS_H

#include <ctime>
#include <vector>
#include <string>
#include <jack/jack.h>
#include <lame/lame.h>
#include "getRecNum.h"
#include "isDir.h"

using namespace std;

typedef jack_default_audio_sample_t sample_t;
static time_t now  = time(0);
static tm     *ltm = localtime(&now);

/* This will make it easy to create a struct if it doesn't already exist */
template<class T>
T* find_or_create(vector<T> *list, T *newT) {
  int i_size = list->size();
  for (int i=0; i<i_size; i++) {
    if ((*list)[i].id == newT->id) return &(*list)[i];
  }

  list->push_back(*newT);
  return &list->back();
}

struct OutPortGroup {
  jack_port_t                 *port_left;
  jack_port_t                 *port_right;
  sample_t *output_left, *output_right;

  unsigned int id                       = 0;
  string       name                     = "";
  string       port_name_left           = "left";
  string       port_name_right          = "right";
  string       hardware_port_path_left  = "";
  string       hardware_port_path_right = "";

  void initialize(jack_client_t *client) {
    port_left  = jack_port_register(client, port_name_left.c_str(),  JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    port_right = jack_port_register(client, port_name_right.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    if ((port_left == NULL) || (port_right == NULL)) {
      fprintf(stderr, "JACK> ERROR> no more JACK ports available\n");
      fprintf(stderr, "             port_left: %s, port_right %s\n",
        port_name_left.c_str(), port_name_right.c_str()
      );
      fprintf(stderr, "             port_left_path: %s, port_right_path %s\n",
        hardware_port_path_left.c_str(), hardware_port_path_right.c_str()
      );
      exit (1);
    }
    else {
      printf("JACK> Jack OutPorts '%s' and '%s' Registered Successfully\n", port_name_left.c_str(), port_name_right.c_str());
    }
  }

  void connect(jack_client_t *client) {
    if (jack_connect(client, jack_port_name(port_left), hardware_port_path_left.c_str())) {
      fprintf (stderr, "JACK> ERROR> cannot connect output ports\n");
    }
    else {
      printf("JACK> Jack OutPort '%s' Connected Successfully\n", port_name_left.c_str());
    }
    if (jack_connect(client, jack_port_name(port_right), hardware_port_path_right.c_str())) {
      fprintf (stderr, "JACK> ERROR> cannot connect output ports\n");
    }
    else {
      printf("JACK> Jack OutPort '%s' Connected Successfully\n", port_name_right.c_str());
    }
  }

  void initPort(jack_nframes_t nframes) {
    output_left  = (sample_t*)jack_port_get_buffer(port_left,  nframes);
    output_right = (sample_t*)jack_port_get_buffer(port_right, nframes);

    for (int i=0; i<nframes; i++) {
      output_left[i]  = 0;
      output_right[i] = 0;
    }
  }
};

struct InPort {
  jack_port_t                 *port;
  sample_t *input;

  unsigned int id                 = 0;
  double       pan                = 0.0; // -1.0: Left, 1.0: Right
  string       name               = "";
  string       hardware_port_path = "";

  void initialize(jack_client_t *client) {
    if (hardware_port_path.substr(0,6) == "system") {
      port = jack_port_register(client, name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

      if (port == NULL) {
        fprintf(stderr, "JACK> ERROR> no more JACK ports available\n");
        fprintf(stderr, "             port: %s\n", name.c_str());
        fprintf(stderr, "             path: %s\n", hardware_port_path.c_str());
        exit (1);
      }
      else {
        printf("JACK> Jack InPort '%s' Registered Successfully\n", name.c_str());
      }
    }
    else if (hardware_port_path.substr(0,6) == "hipbox") {
      printf("JACK> Port %s Needs to be set up with AEM, not with jack.\n", name.c_str());
    }
  }

  void connect(jack_client_t *client) {
    if (hardware_port_path.substr(0,6) == "system") {
      if (jack_connect(client, hardware_port_path.c_str(), jack_port_name(port))) {
        fprintf (stderr, "JACK> ERROR> cannot connect input ports\n");
      }
      else {
        printf("JACK> Jack InPort '%s' Connected Successfully\n", name.c_str());
      }
    }
    else if (hardware_port_path.substr(0,6) == "hipbox") {
      printf("JACK> Port %s Needs to be set up with AEM, not with jack.\n", name.c_str());
    }
  }

  void initPort(jack_nframes_t nframes) {
    if (checkRealPort()) {
      input = (sample_t*)jack_port_get_buffer(port, nframes);
    }
  }

  bool checkRealPort() {
    if (hardware_port_path.substr(0,6) == "system")
      return true;
    return false;
  }

};

struct InPortGroup {
  unsigned int    id        = 0;
  unsigned int    osc_port  = 5000;
  string          name      = "";
  bool            is_record = false;
  bool            is_jc_on  = false;  // is jack_capture running
  vector<InPort*> ports;

  void initialize(jack_client_t *client) {
    int size = ports.size();
    for (int i=0; i<size; i++) {
      ports[i]->initialize(client);
    }
  }

  void connect(jack_client_t *client) {
    int size = ports.size();
    for (int i=0; i<size; i++) {
      ports[i]->connect(client);
    }
  }

  void init_jack_capture(jack_client_t *client) {
    if (is_record && !is_jc_on) {
      is_jc_on = true;

      string cmd  = "";
      cmd += "jack_capture";
      cmd += " --no-stdin";                     // dont read keystrokes from terminal
      cmd += " --jack-transport";               // dont start recording till jack says so
      cmd += " --disable-meter";                // disable terminal graphic meter
      cmd += " --bitdepth 16";                  // 16, 24
      cmd += " --channels 1";                   // mono=1, stereo=2
      cmd += " --mp3";                          // record as mp3
      cmd += " --osc " + to_string(osc_port);   // osc port for communcation

      /* ports to record */
      int i_size = ports.size();
      for (int i=0; i<i_size; i++)
        cmd += " --port " + ports[i]->hardware_port_path;
      /* -- -- */

      /* filename and directory */
      char num_of_rec[3];
      sprintf(num_of_rec, "%02d", getRecNum());
      string num_of_rec_s = string(num_of_rec);

      if (!isDir("recordings/"+num_of_rec_s)) {
        PRINTD("\n\nAEM> 'recordings/%s' DIR doesn't exist.. Creating Directory\n\n", num_of_rec_s.c_str());
        string cmd = "mkdir -p recordings/"+num_of_rec_s;
        system(cmd.c_str());
      }

      string filename = "";
      filename += "recordings/" + num_of_rec_s + "/";
      filename += to_string(1900+ltm->tm_year) + to_string(1+ltm->tm_mon) + to_string(ltm->tm_mday);
      filename += "_" + name + "_" + num_of_rec_s + ".mp3";

      cmd += " " + filename;
      /* -- -- */

      cmd += " &";
      PRINTD("JACKCAPTURE> start cmd: %s", cmd.c_str());
      system(cmd.c_str());
    }
  }

  void stop_jack_capture(/* osc client */) {
    is_jc_on = false;
    /**
     * send osc message to ports 5000+(i+1)
     * to stop jack capture
     */
  }
};

struct Channel {
  InPortGroup *in_port_group;
  unsigned int id      = 0;
  double       gain    = 0.0;
  double       pan     = 0.0;  // -1.0: Left, 1.0: Right
  bool         is_mute = false;
};

struct Mixer {
  vector<Channel> channels;
  OutPortGroup    *out_port_group;
  unsigned int    id   = 0;
  double          gain = 0.0;
};

struct RecFile {
  static time_t       now;
  static tm           *ltm;
  static unsigned int recNum;

  unsigned int id          = 0;
  string       filename    = "";
  bool         stop_record = false;
  sample_t     *left;
  sample_t     *right;
  InPortGroup  *in_port_group;
  int          bufsize;
  string       num_of_rec_s;

  // LAME
  lame_global_flags *lame;
  FILE              *mp3file = NULL;
  int               size     = 0;
  unsigned char     *mp3buf;
  int               mp3bufsize;
  // --

  RecFile(InPortGroup *in_port_group) {
    this->in_port_group = in_port_group;
    this->id            = in_port_group->id;
    create_file_name(in_port_group->name);
  }

  void create_file_name(string base_name) {
    char num_of_rec[3];
    sprintf(num_of_rec, "%02d", recNum);
    num_of_rec_s = string(num_of_rec);

    filename  = "recordings/" + num_of_rec_s + "/";
    filename += to_string(1900+ltm->tm_year) + to_string(1+ltm->tm_mon) + to_string(ltm->tm_mday);
    filename += "_" + base_name + "_" + num_of_rec_s + ".mp3";
  }

  /* This is where the recording starts */
  int init(jack_client_t *client) {
    mp3bufsize = 4096*20;
    mp3buf     = (unsigned char*)malloc(mp3bufsize);  //mp3bufsize
    bufsize    = jack_get_buffer_size(client);
    left       = (sample_t*)malloc(bufsize*sizeof(sample_t));
    right      = (sample_t*)malloc(bufsize*sizeof(sample_t));

    lame = lame_init();
    if(lame==NULL){
      PRINTD("\n\nLAME> lame_init failed.\n\n");
      return 0;
    }

    lame_set_num_channels(lame, 2); // 2 channel for left and right
    lame_set_in_samplerate(lame,(int)jack_get_sample_rate(client));
    lame_set_out_samplerate(lame,(int)jack_get_sample_rate(client));
    lame_set_quality(lame,2); // 0 best - 9 worst

    {
      int ret = lame_init_params(lame);
      if(ret<0){
        PRINTD("\n\nLAME> Illegal parameters for lame. (%d) \n\n", ret);
        return 0;
      }
    }

    PRINTD("\n\nLAME> Filename: %s \n\n", filename.c_str());
    if (!isDir("recordings/"+num_of_rec_s)) {
      PRINTD("\n\nAEM> 'recordings/%s' DIR doesn't exist.. Creating Directory\n\n", num_of_rec_s.c_str());
      string cmd = "mkdir -p recordings/"+num_of_rec_s;
      system(cmd.c_str());
    }
    mp3file = fopen(filename.c_str(),"w");
    if(mp3file == NULL){
      PRINTD("\n\nLAME> Can not open file \"%s\" \n\n", filename.c_str());
      return 0;
    }

    return 1;
  }

  void record(jack_nframes_t nframes, bool &is_recording, bool &init_record) {
    if (is_recording) {
      stop_record = true; // will need to stop recording after we are done

      // Initialize data
      for (int l=0; l<nframes; l++) {
        left[l]  = 0;
        right[l] = 0;
      }

      /* Loop through all InPorts in group */
      int i_size = in_port_group->ports.size();
      for (int i=0; i<i_size; i++) {
        if (in_port_group->ports[i]->checkRealPort()) {
          InPort *in_port = in_port_group->ports[i];

          double in_port_pan_left = 1, in_port_pan_right = 1;
          if (in_port->pan < 0)
            in_port_pan_right += in_port->pan;
          else
            in_port_pan_left -= in_port->pan;

          for (int l=0; l<nframes; l++) {
                     //    raw audio           in port pan
            left[l]  += (in_port->input[l] * in_port_pan_left);
            right[l] += (in_port->input[l] * in_port_pan_right);
          }
        }
      }
      size = lame_encode_buffer_float(lame, left, right, (size_t)nframes, mp3buf, mp3bufsize);
      if (size > 0) fwrite(mp3buf,size,1,mp3file);
    }
    else {
      if (stop_record) {
        stop_record = false;

        size = lame_encode_flush(lame, mp3buf, mp3bufsize);
        if (size > 0) fwrite(mp3buf,size,1,mp3file);

        lame_close(lame);
        fclose(mp3file);

        init_record = false;
      }
    }
  }
};
time_t       RecFile::now    = time(0);
tm           *RecFile::ltm   = localtime(&RecFile::now);
unsigned int RecFile::recNum = getRecNum();

#endif
