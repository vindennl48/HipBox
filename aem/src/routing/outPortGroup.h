#ifndef OUTPORTGROUP_H
#define OUTPORTGROUP_H

#include <jack/jack.h>
#include <string>
#include "../extra/json.h"
#include "../extra/print.h"

using json = nlohmann::json;
using namespace std;

struct OutPortGroup {
  jack_port_t                 *port_left;
  jack_port_t                 *port_right;
  jack_default_audio_sample_t *output_left, *output_right;

  static OutPortGroup* create_from_json(json *jout_port_group);

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
      PRINT("outPortGroup.initialize> no more JACK ports available");
      PRINT("                         port_left: %s, port_right %s",
        port_name_left.c_str(), port_name_right.c_str()
      );
      PRINT("                         port_left_path: %s, port_right_path %s",
        hardware_port_path_left.c_str(), hardware_port_path_right.c_str()
      );
      exit (1);
    }
    else {
      PRINT("outPortGroup.initialize> Jack OutPorts '%s' and '%s' Registered Successfully", port_name_left.c_str(), port_name_right.c_str());
    }
  }

  void connect(jack_client_t *client) {
    if (jack_connect(client, jack_port_name(port_left), hardware_port_path_left.c_str())) {
      PRINT("outPortGroup.connect> Cannot connect output ports for port '%s'", port_name_left.c_str());
    }
    else {
      PRINT("outPortGroup.connect> Jack OutPort '%s' Connected Successfully", port_name_left.c_str());
    }
    if (jack_connect(client, jack_port_name(port_right), hardware_port_path_right.c_str())) {
      PRINT("outPortGroup.connect> cannot connect output ports for port '%s'", port_name_right.c_str());
    }
    else {
      PRINT("outPortGroup.connect> Jack OutPort '%s' Connected Successfully", port_name_right.c_str());
    }
  }

  void initPort(jack_nframes_t nframes) {
    output_left  = (jack_default_audio_sample_t*)jack_port_get_buffer(port_left,  nframes);
    output_right = (jack_default_audio_sample_t*)jack_port_get_buffer(port_right, nframes);

    for (int i=0; i<nframes; i++) {
      output_left[i]  = 0;
      output_right[i] = 0;
    }
  }
};

#endif
