#include <unistd.h>
#include <string>
#include <vector>
#include <jack/jack.h>
#include "db.h"

// These structs are for the hipbox mixer.

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

#define NUM_MIXER_CHANNELS  8

struct Mixer {
  Channel      channels[NUM_MIXER_CHANNELS];
  OutPortGroup *out_port_group;
  bool         is_active = false;
  double       gain      = 0.0;
};

// End hipbox mixer structs

// These are based on an 8 input / 10 output audio interface
#define NUM_IN_PORTS         8
#define NUM_IN_PORT_GROUPS   8
#define NUM_OUT_PORT_GROUPS  5

jack_client_t *client;
InPort        in_ports[NUM_IN_PORTS];
InPortGroup   in_port_groups[NUM_IN_PORT_GROUPS];
OutPortGroup  out_port_groups[NUM_OUT_PORT_GROUPS];
Mixer         mixers[NUM_OUT_PORT_GROUPS];

int
process (jack_nframes_t nframes, void *arg) {

  /* Loop through all Mixers */
  for (int i=0; i<NUM_OUT_PORT_GROUPS; i++) {
    Mixer        *mixer          = &mixers[i];
    OutPortGroup *out_port_group = mixer->out_port_group;

    if (mixer->is_active) {
      out_port_group->initPort(nframes);

      /* Loop through all Channels in Mixer */
      for (int j=0; j<NUM_MIXER_CHANNELS; j++) {
        Channel     *channel       = &mixer->channels[j];
        InPortGroup *in_port_group = channel->in_port_group;

        if (channel->is_active && !channel->is_mute) {

          /* Loop through all InPorts in Channel */
          int in_port_group_size = in_port_group->ports.size();
          for (int k=0; k<in_port_group_size; k++) {
            InPort *in_port = in_port_group->ports[k];
            in_port->initPort(nframes);

            double in_port_pan_left = 1, in_port_pan_right = 1;
            if (in_port->pan < 0)
              in_port_pan_right += in_port->pan;
            else
              in_port_pan_left -= in_port->pan;

            double channel_pan_left = 1, channel_pan_right = 1;
            if (channel->pan < 0)
              channel_pan_right += channel->pan;
            else
              channel_pan_left -= channel->pan;

            for (int l=0; l<nframes; l++) {
                                              //    raw audio           in port pan          channel pan                 channel gain             mixer gain
              out_port_group->output_left[l]  += (((in_port->input[l] * in_port_pan_left)  * channel_pan_left)  * db2lin(channel->gain)) * db2lin(mixer->gain);
              out_port_group->output_right[l] += (((in_port->input[l] * in_port_pan_right) * channel_pan_right) * db2lin(channel->gain)) * db2lin(mixer->gain);
            }
          }

        }
      }

    }
  }

	return 0;      
}

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void
jack_shutdown (void *arg)
{
	exit (1);
}

int
main (int argc, char *argv[])
{
  const char     *client_name = "hipbox";
	const char     *server_name = NULL;
	jack_options_t options     = JackNullOption;
	jack_status_t  status;
	
	/* open a client connection to the JACK server */

	client = jack_client_open (client_name, options, &status, server_name);
	if (client == NULL) {
		fprintf (stderr, "jack_client_open() failed, "
			 "status = 0x%2.0x\n", status);
		if (status & JackServerFailed) {
			fprintf (stderr, "Unable to connect to JACK server\n");
		}
		exit (1);
	}
	if (status & JackServerStarted) {
		fprintf (stderr, "JACK server started\n");
	}
	if (status & JackNameNotUnique) {
		client_name = jack_get_client_name(client);
		fprintf (stderr, "unique name `%s' assigned\n", client_name);
	}

	/* tell the JACK server to call `process()' whenever
	   there is work to be done.
	*/

	jack_set_process_callback (client, process, 0);

	/* tell the JACK server to call `jack_shutdown()' if
	   it ever shuts down, either entirely, or if it
	   just decides to stop calling us.
	*/

	jack_on_shutdown (client, jack_shutdown, 0);

	/* display the current sample rate. 
	 */

	printf ("engine sample rate: %" PRIu32 "\n",
		jack_get_sample_rate (client));

	/* create ports */


  /*
   * Setup will eventually be a JSON string sent over OSC
   * A test file "json_test.json" has been laid out in the
   * root of this dir.
   * */

  // Setting up all the InPorts and OutPortGroups.  This will
  // all be done by a JSON parser in the future, but for sake
  // of testing, here goes..

  in_ports[0].port_name          = "mic";
  in_ports[0].pan                = 0;
  in_ports[0].hardware_port_path = "system:capture_1";
  //in_ports[0].initialize(client);
  in_ports[1].port_name          = "mic2";
  in_ports[1].hardware_port_path = "system:capture_2";
  //in_ports[1].initialize(client);

  in_port_groups[0].ports.push_back(&in_ports[0]);
  in_port_groups[0].initialize(client);

  out_port_groups[0].port_name_left           = "mitch_L";
  out_port_groups[0].port_name_right          = "mitch_R";
  out_port_groups[0].hardware_port_path_left  = "system:playback_1";
  out_port_groups[0].hardware_port_path_right = "system:playback_2";
  out_port_groups[0].initialize(client);

  mixers[0].out_port_group            = &out_port_groups[0];
  mixers[0].channels[0].in_port_group = &in_port_groups[0];
  mixers[0].channels[0].gain          = 0;
  mixers[0].channels[0].pan           = 0;
  mixers[0].channels[0].is_mute       = false;
  mixers[0].channels[0].is_active     = true;
  mixers[0].gain                      = 0;
  mixers[0].is_active                 = true;


  // End setting up InPorts and OutPortGroups.


	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */

	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		exit (1);
	}

	/* Connect the ports.  You can't do this before the client is
	 * activated, because we can't make connections to clients
	 * that aren't running.  Note the confusing (but necessary)
	 * orientation of the driver backend ports: playback ports are
	 * "input" to the backend, and capture ports are "output" from
	 * it.
	 */

  for (int i=0; i<NUM_IN_PORT_GROUPS; i++) {
    in_port_groups[i].connect(client);
  }
  for (int i=0; i<NUM_OUT_PORT_GROUPS; i++) {
    out_port_groups[i].connect(client);
  }

	/* keep running until stopped by the user */

	sleep (-1);

	/* this is never reached but if the program
	   had some other way to exit besides being killed,
	   they would be important to call.
	*/

	jack_client_close (client);
	exit (0);
}
