#ifndef INPORT_H
#define INPORT_H

#include <string>
#include <jack/jack.h>
#include "../extra/print.h"
#include "../extra/json.h"

using json = nlohmann::json;
using namespace std;

struct InPort {
  jack_port_t                 *port;
  jack_default_audio_sample_t *input;

  unsigned int id                 = 0;
  double       pan                = 0.0; // -1.0: Left, 1.0: Right
  string       name               = "";
  string       hardware_port_path = "";

  static InPort* create_from_json(json *jin_port);

  void initialize(jack_client_t *client) {
    if (hardware_port_path.substr(0,6) == "system") {
      port = jack_port_register(client, name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

      if (port == NULL) {
        PRINT("routing.inPort> no more JACK ports available");
        PRINT("                port: %s", name.c_str());
        PRINT("                path: %s", hardware_port_path.c_str());
        exit (1);
      }
      else {
        PRINT("routing.inPort> Jack InPort '%s' Registered Successfully", name.c_str());
      }
    }
    else if (hardware_port_path.substr(0,6) == "hipbox") {
      PRINT("routing.inPort> Port %s Needs to be set up with AEM, not with jack.", name.c_str());
    }
  }

  void connect(jack_client_t *client) {
    if (hardware_port_path.substr(0,6) == "system") {
      if (jack_connect(client, hardware_port_path.c_str(), jack_port_name(port))) {
        PRINT("routing.inPort> Cannot connect input ports for port '%s'", name.c_str());
      }
      else {
        PRINT("routing.inPort> Jack InPort '%s' Connected Successfully", name.c_str());
      }
    }
    else if (hardware_port_path.substr(0,6) == "hipbox") {
      PRINT("routing.inPort> Port %s Needs to be set up with AEM, not with jack.", name.c_str());
    }
  }

  void initPort(jack_nframes_t nframes) {
    if (hardware_port_path.substr(0,6) == "system") {
      input = (jack_default_audio_sample_t*)jack_port_get_buffer(port, nframes);
    }
  }

  bool checkRealPort() {
    if (hardware_port_path.substr(0,6) == "system")
      return true;
    return false;
  }

};

#endif
