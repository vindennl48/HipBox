#include <vector>
#include <string>
#include <jack/jack.h>
#include <lo/lo.h>
#include "json.h"
#include "db.h"
#include "hipbox.h"

using json = nlohmann::json;

// -- JACK --
struct OutPortGroup {
  jack_port_t                 *port_left;
  jack_port_t                 *port_right;
  jack_default_audio_sample_t *output_left, *output_right;

  std::string name;
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
  jack_port_t                 *port;
  jack_default_audio_sample_t *input;

  double      pan       = 0.0; // -1.0: Left, 1.0: Right
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
  std::string          group_name;
  std::vector<InPort*> ports;
  bool                 is_active = false;

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

jack_client_t *client;
InPort        in_ports[NUM_IN_PORTS];
InPortGroup   in_port_groups[NUM_IN_PORT_GROUPS];
OutPortGroup  out_port_groups[NUM_OUT_PORT_GROUPS];
Mixer         mixers[NUM_OUT_PORT_GROUPS];

int process (jack_nframes_t nframes, void *arg) {

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
void jack_shutdown (void *arg) {
	exit (1);
}

void start_jack() {
  const char     *client_name    = "hipbox";
	const char     *server_name    = NULL;
	jack_options_t options         = JackNullOption;
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
}

void stop_jack() {
	jack_client_close(client);
}

// --

// -- OSC --
lo_server_thread server_thread = NULL;

int rails_handler(const char *path, const char *types, lo_arg **argv, int argc,
		              lo_message msg, void *user_data)
{
  auto j3         = json::parse((std::string)&argv[0]->s);
  bool is_restart = false;
  int  size       = 0;
  int  id         = 0;

  printf("----> rails_handler\n");
  //printf("----> Json: %s \n", j3.dump().c_str());
  //printf("----> %s \n", j3["inPorts"].dump().c_str());
  printf("----> %s\n", j3.dump(4).c_str());

  /* Updates that require jack restart */
  if (j3.count("in_port_groups")) {
    printf("----> InPortGroupss\n");

    // Reset all in-port groups
    for (int i=0; i<NUM_IN_PORT_GROUPS; i++) {
      InPortGroup in_port_group;
      in_port_groups[i] = in_port_group;
    }

    size = j3["in_port_groups"].size();
    for (int i=0; i<size; i++) {
      in_port_groups[i].group_name = (std::string) j3["in_port_groups"][i]["name"];
    }

    is_restart = true;

    // Test
    for (int i=0; i<NUM_IN_PORT_GROUPS; i++) {
      printf("TEST> InPortGroup Name: %s\n", in_port_groups[i].group_name.c_str());
    }
  }

  if (j3.count("in_ports")) {
    // Reset all in-ports
    for (int i=0; i<NUM_IN_PORTS; i++) {
      InPort in_port;
      in_ports[i] = in_port;
    }

    size = j3["in_ports"].size();
    size = size > NUM_IN_PORTS ? NUM_IN_PORTS : size;
    for (int i=0; i<size; i++) {
      in_ports[i].port_name          = (std::string) j3["in_ports"][i]["name"];
      in_ports[i].hardware_port_path = (std::string) j3["in_ports"][i]["path"];
      in_ports[i].pan                = std::stod((std::string) j3["in_ports"][i]["pan"]);

      // Assign in-port to in-port-group if it exists
      if (j3["in_ports"][i].count("in_port_group")) {
        std::string in_port_group_name = j3["in_ports"][i]["in_port_group"];
        for (int j=0; j<NUM_IN_PORT_GROUPS; j++) {
          if (in_port_group_name == in_port_groups[j].group_name) {
            in_port_groups[j].ports.push_back(&in_ports[i]);
          }
        }
      }
    }

    is_restart = true;

    // Test
    for (int i=0; i<NUM_IN_PORTS; i++) {
      printf("TEST> InPort Name: %s, Path: %s\n",
        in_ports[i].port_name.c_str(),
        in_ports[i].hardware_port_path.c_str()
      );
    }
  }

//  if (j3.count("mixers")) {
//    // int size = j3["inPorts"].size();
//    // printf("----> Size: %i \n", size);
//    // for (int i=0; i<size; i++) {
//    //   in_ports[i].port_name = (std::string)j3["inPorts"][i]["name"];
//    //   printf("----> InPorts %i - Name: %s \n", i, in_ports[i].port_name.c_str());
//    //   //std::cout << "----> InPorts: " << (std::string)j3["inPorts"][i]["name"] << std::endl;
//    // }
//    is_restart = true;
//  }
//
//  /* END: Updates that require jack restart */

  return 0;
}

int wildcard_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 lo_message msg, void *user_data) {
  fprintf(stderr, "Warning: unhandled OSC message: '%s' with args '%s'.\n", path, types);

  return -1;
}

int ping_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 lo_message msg, void *user_data)
{
	lo_address src = lo_message_get_source( msg );
	lo_server serv = (lo_server)user_data;
	int result;
	
	// Display the address the ping came from
	//if (verbose) {
  char *url = lo_address_get_url(src);
  printf( "Got ping from: %s\n", url);
  free(url);
	//}

	// Send back reply
	result = lo_send_from( src, serv, LO_TT_IMMEDIATE, "/pong", "" );
	if (result<1) fprintf(stderr, "Error: sending reply failed: %s\n", lo_address_errstr(src));

    return 0;
}


void error_handler(int num, const char *msg, const char *path) {
  fprintf(stderr, "LibLO server error %d in path %s: %s\n", num, path, msg);
  fflush(stdout);
}

void start_osc() {
  lo_server_thread server_thread = NULL;
  lo_server        serv          = NULL;

  // Create new server
  server_thread = lo_server_thread_new(OSC_PORT, error_handler);
  if (!server_thread) exit(1);

  // Add the methods
  serv = lo_server_thread_get_server( server_thread );
  //lo_server_thread_add_method(server_thread, "/mixer/get_channel_count", "",   get_channel_count_handler, serv);
  //lo_server_thread_add_method(server_thread, "/mixer/channel/set_gain",  "if", set_gain_handler,          serv);
  //lo_server_thread_add_method(server_thread, "/mixer/channel/get_gain",  "i",  get_gain_handler,          serv);
  //lo_server_thread_add_method(server_thread, "/mixer/channel/get_label", "i",  get_label_handler,         serv);
  //lo_server_thread_add_method(server_thread, "/mixer/channel/set_label", "is", set_label_handler,         serv);
  lo_server_thread_add_method(server_thread, "/rails", "s", rails_handler, serv);
  lo_server_thread_add_method(server_thread, "/ping",  "",  ping_handler,  serv);

  // add method that will match any path and args
  lo_server_thread_add_method(server_thread, NULL, NULL, wildcard_handler, serv);

  // Start the thread
  lo_server_thread_start(server_thread);

  char *url = lo_server_thread_get_url( server_thread );
  printf( "OSC server URL: %s\n", url );
  free(url);
}

void stop_osc() {
  lo_server_thread_stop(server_thread);
  lo_server_thread_free(server_thread);
}

// --
