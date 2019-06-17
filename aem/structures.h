#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>

// These structs are for the hipbox mixer.

#define NUM_MIXER_CHANNELS   8

struct OutPortGroup {
  jack_port_t                 *port_left;
  jack_port_t                 *port_right;
  jack_default_audio_sample_t *output_left, *output_right;

  std::string port_name_left;
  std::string port_name_right;
  std::string hardware_port_path_left;
  std::string hardware_port_path_right;
  bool        is_active = false;

  void initialize(jack_client_t *client) {
    port_left  = jack_port_register(client, port_name_left.c_str(),  JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    port_right = jack_port_register(client, port_name_right.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    if ((port_left == NULL) || (port_right == NULL)) {
      fprintf(stderr, "no more JACK ports available\n");
      exit (1);
    }

    is_active = true;
  }

  void connect(jack_client_t *client) {
    if (is_active) {
      if (jack_connect(client, jack_port_name(port_left), hardware_port_path_left.c_str())) {
        fprintf (stderr, "cannot connect input ports\n");
      }
      if (jack_connect(client, jack_port_name(port_right), hardware_port_path_right.c_str())) {
        fprintf (stderr, "cannot connect input ports\n");
      }
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

struct InPort {
  jack_port_t *port;
  jack_default_audio_sample_t *input;

  double      pan       = 0.0;  // -1.0: Left, 1.0: Right
  bool        is_active = false;
  std::string port_name;
  std::string hardware_port_path;

  void initialize(jack_client_t *client) {
    port = jack_port_register(client, port_name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

    if (port == NULL) {
      fprintf(stderr, "no more JACK ports available\n");
      exit (1);
    }

    is_active = true;
  }

  void connect(jack_client_t *client) {
    if (is_active) {
      if (jack_connect(client, hardware_port_path.c_str(), jack_port_name(port))) {
        fprintf (stderr, "cannot connect input ports\n");
      }
    }
  }

  void initPort(jack_nframes_t nframes) {
    input = (jack_default_audio_sample_t*)jack_port_get_buffer(port, nframes);
  }
};

struct InPortGroup {
  std::vector<InPort*> ports;
  bool is_active = false;

  void initialize(jack_client_t *client) {
    int size = ports.size();
    for (int i=0; i<size; i++) {
      ports[i]->initialize(client);
    }

    is_active = true;
  }

  void connect(jack_client_t *client) {
    int size = ports.size();
    for (int i=0; i<size; i++) {
      ports[i]->connect(client);
    }
  }
};

struct Channel {
  InPortGroup *in_port_group;
  double      gain      = 0.0;
  double      pan       = 0.0;  // -1.0: Left, 1.0: Right
  bool        is_mute   = false;
  bool        is_active = false;
};

struct Mixer {
  Channel      channels[NUM_MIXER_CHANNELS];
  OutPortGroup *out_port_group;
  bool         is_active = false;
  double       gain      = 0.0;
};

// End hipbox mixer structs

#endif
