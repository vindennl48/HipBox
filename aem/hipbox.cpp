#include <vector>
#include <string>
#include <jack/jack.h>
#include <lo/lo.h>
#include "json.h"
#include "db.h"
#include "hipbox.h"

using json = nlohmann::json;
using namespace std;

// #define vector std::vector
// #define string std::string
// #define stod   std::stod

// -- JACK --

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
  jack_default_audio_sample_t *output_left, *output_right;

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

struct InPortGroup {
  unsigned int    id   = 0;
  string          name = "";
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

jack_client_t        *client;
vector<InPort>       in_ports;
vector<InPortGroup>  in_port_groups;
vector<OutPortGroup> out_port_groups;
vector<Mixer>        mixers;

int process (jack_nframes_t nframes, void *arg) {

  /* Loop through all Mixers */
  int i_size = mixers.size();
  for (int i=0; i<i_size; i++) {
    Mixer        *mixer          = &mixers[i];
    OutPortGroup *out_port_group = mixer->out_port_group;

    out_port_group->initPort(nframes);

    /* Loop through all Channels in Mixer */
    int j_size = mixer->channels.size();
    for (int j=0; j<j_size; j++) {
      Channel     *channel       = &mixer->channels[j];
      InPortGroup *in_port_group = channel->in_port_group;

      if (!channel->is_mute) {

        /* Loop through all InPorts in Channel */
        int k_size = in_port_group->ports.size();
        for (int k=0; k<k_size; k++) {
          if (in_port_group->ports[k]->checkRealPort()) {
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
              //out_port_group->output_left[l]  += (((in_port->input[l] * in_port_pan_left)  * channel_pan_left)  * db2lin(channel->gain)) * db2lin(mixer->gain);
              //out_port_group->output_right[l] += (((in_port->input[l] * in_port_pan_right) * channel_pan_right) * db2lin(channel->gain)) * db2lin(mixer->gain);
              out_port_group->output_left[l]  += (((in_port->input[l] * in_port_pan_left)  * channel_pan_left)  * db2lin(0)) * db2lin(0);
              out_port_group->output_right[l] += (((in_port->input[l] * in_port_pan_right) * channel_pan_right) * db2lin(0)) * db2lin(0);
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
  const char     *client_name = "hipbox";
	const char     *server_name = NULL;
	jack_options_t options      = JackNullOption;
	jack_status_t  status;
	
	/* open a client connection to the JACK server */
	client = jack_client_open (client_name, options, &status, server_name);
	if (client == NULL) {
		fprintf (stderr, "JACK> ERROR> jack_client_open() failed, "
			 "status = 0x%2.0x\n", status);
		if (status & JackServerFailed) {
			fprintf (stderr, "JACK> ERROR> Unable to connect to JACK server\n");
		}
		exit (1);
	}
	if (status & JackServerStarted) {
		fprintf (stderr, "JACK> JACK server started\n");
	}
	if (status & JackNameNotUnique) {
		client_name = jack_get_client_name(client);
		fprintf (stderr, "JACK> unique name `%s' assigned\n", client_name);
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

	/* initialize ports */
  int i_size = in_port_groups.size();
  for (int i=0; i<i_size; i++) {
    in_port_groups[i].initialize(client);
  }
  i_size = out_port_groups.size();
  for (int i=0; i<i_size; i++) {
    out_port_groups[i].initialize(client);
  }

  // /*
  //  * Setup will eventually be a JSON string sent over OSC
  //  * A test file "json_test.json" has been laid out in the
  //  * root of this dir.
  //  * */

  // // Setting up all the InPorts and OutPortGroups.  This will
  // // all be done by a JSON parser in the future, but for sake
  // // of testing, here goes..

  // in_ports[0].name          = "mic";
  // in_ports[0].pan                = 0;
  // in_ports[0].hardware_port_path = "system:capture_1";
  // //in_ports[0].initialize(client);
  // in_ports[1].name          = "mic2";
  // in_ports[1].hardware_port_path = "system:capture_2";
  // //in_ports[1].initialize(client);

  // in_port_groups[0].ports.push_back(&in_ports[0]);
  // in_port_groups[0].initialize(client);

  // out_port_groups[0].port_name_left           = "mitch_L";
  // out_port_groups[0].port_name_right          = "mitch_R";
  // out_port_groups[0].hardware_port_path_left  = "system:playback_1";
  // out_port_groups[0].hardware_port_path_right = "system:playback_2";
  // out_port_groups[0].initialize(client);

  // mixers[0].out_port_group            = &out_port_groups[0];
  // mixers[0].channels[0].in_port_group = &in_port_groups[0];
  // mixers[0].channels[0].gain          = 0;
  // mixers[0].channels[0].pan           = 0;
  // mixers[0].channels[0].is_mute       = false;
  // mixers[0].gain                      = 0;


  // End setting up InPorts and OutPortGroups.


	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */

	if (jack_activate (client)) {
		fprintf (stderr, "JACK> ERROR> cannot activate client");
		exit (1);
	}

	/* Connect the ports.  You can't do this before the client is
	 * activated, because we can't make connections to clients
	 * that aren't running.  Note the confusing (but necessary)
	 * orientation of the driver backend ports: playback ports are
	 * "input" to the backend, and capture ports are "output" from
	 * it.
	 */

  i_size = in_port_groups.size();
  for (int i=0; i<i_size; i++) {
    in_port_groups[i].connect(client);
  }
  i_size = out_port_groups.size();
  for (int i=0; i<i_size; i++) {
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
  auto j3 = json::parse((string)&argv[0]->s);

  PRINTD("----> rails_handler\n");

  /* Dump entire json string */
  //PRINTD("----> %s\n", j3.dump(4).c_str());

  if (j3.count("mixers")) {
    auto *mixers_p = &j3["mixers"];

    /* Reset */
    //stop_jack();
    in_ports.clear();
    in_port_groups.clear();
    out_port_groups.clear();
    mixers.clear();

    in_ports.reserve(DEFAULT_VECTOR_SIZE);
    in_port_groups.reserve(DEFAULT_VECTOR_SIZE);
    out_port_groups.reserve(DEFAULT_VECTOR_SIZE);
    mixers.reserve(DEFAULT_VECTOR_SIZE);
    /* -- */

    int i_size = mixers_p->size();
    for (int i=0; i<i_size; i++) {
      auto *mixer_p          = &(*mixers_p)[i];
      auto *out_port_group_p = &(*mixers_p)[i]["out_port_group"];
      auto *channels_p       = &(*mixers_p)[i]["channels"];

      OutPortGroup out_port_group;
      out_port_group.id                       = (*out_port_group_p)["id"];
      out_port_group.name                     = (*out_port_group_p)["name"];
      out_port_group.port_name_left           = (*out_port_group_p)["ports"][0]["name"];
      out_port_group.port_name_right          = (*out_port_group_p)["ports"][1]["name"];
      out_port_group.hardware_port_path_left  = (*out_port_group_p)["ports"][0]["path"];
      out_port_group.hardware_port_path_right = (*out_port_group_p)["ports"][1]["path"];

      Mixer mixer;
      mixer.id             = (*mixer_p)["id"];
      mixer.gain           = stod((string)(*mixer_p)["gain"]);
      mixer.out_port_group = find_or_create<OutPortGroup>(&out_port_groups, &out_port_group);

      int j_size = channels_p->size();
      for (int j=0; j<j_size; j++) {
        auto *channel_p = &(*channels_p)[j];

        InPortGroup in_port_group;
        in_port_group.id   = (*channel_p)["port_group"]["id"];
        in_port_group.name = (*channel_p)["port_group"]["name"];

        int k_size = (*channel_p)["port_group"]["ports"].size();
        for (int k=0; k<k_size; k++) {
          auto *port_p = &(*channel_p)["port_group"]["ports"][k];

          InPort in_port;
          in_port.id                 = (*port_p)["id"];
          in_port.pan                = stod((string)(*port_p)["pan"]);
          in_port.name               = (*port_p)["name"];
          in_port.hardware_port_path = (*port_p)["path"];

          in_port_group.ports.push_back(
            find_or_create<InPort>(&in_ports, &in_port)
          );
        }

        Channel channel;
        channel.id            = (*channel_p)["id"];
        channel.gain          = stod((string)(*channel_p)["gain"]);
        channel.pan           = stod((string)(*channel_p)["pan"]);
        channel.is_mute       = (*channel_p)["is_mute"];
        channel.in_port_group = find_or_create<InPortGroup>(&in_port_groups, &in_port_group);

        mixer.channels.push_back(channel);
      }

      mixers.push_back(mixer);
    }

    PRINTD("----> Starting Jack Service\n");
    start_jack();
    PRINTD("----> Jack Service Up and Running\n");

   /* All the testing to confirm this works
    * 
    * // Test
    * i_size = in_port_groups.size();
    * PRINTD("TEST> InPortGroups Size: %i\n", i_size);
    * for (int i=0; i<i_size; i++) {
    *   PRINTD("TEST> InPortGroup Name: %s\n", in_port_groups[i].name.c_str());
    * }
    * 
    * 
    * // Test
    * i_size = in_ports.size();
    * PRINTD("TEST> InPorts Size: %i\n", i_size);
    * for (int i=0; i<i_size; i++) {
    *   PRINTD("TEST> InPort Name: %s, Path: %s\n",
    *     in_ports[i].name.c_str(),
    *     in_ports[i].hardware_port_path.c_str()
    *   );
    * }
    * 
    * // Test for port groups
    * i_size = in_port_groups.size();
    * PRINTD("TEST> InPortGroups Size: %i\n", i_size);
    * for (int i=0; i<i_size; i++) {
    *   PRINTD("TEST> InPortGroup Ports Size: %i\n", (int)in_port_groups[i].ports.size());
    *   // if (in_port_groups[i].ports.size() > 0) {
    *     PRINTD("TEST> InPortGroup FirstPortName: %s\n", in_port_groups[i].ports[0]->hardware_port_path.c_str());
    *   // }
    * }
    * 
    * // Test
    * i_size = out_port_groups.size();
    * PRINTD("TEST> OutPortGroups Size: %i\n", i_size);
    * for (int i=0; i<i_size; i++) {
    *   PRINTD("TEST> OutPortGroup LeftName: %s, RightName: %s, LeftPath: %s, RightPath: %s\n",
    *     out_port_groups[i].port_name_left.c_str(),
    *     out_port_groups[i].port_name_right.c_str(),
    *     out_port_groups[i].hardware_port_path_left.c_str(),
    *     out_port_groups[i].hardware_port_path_right.c_str()
    *   );
    * }
    */
    
  } // -- END j3.count("mixers") --

  //if (j3.count("channels")) {
    //auto *channels_p = &j3["channels"];
  //} // -- END j3.count("channels") --

  return 0;
}

int wildcard_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 lo_message msg, void *user_data) {
  fprintf(stderr, "OSC> ERROR> Warning: unhandled OSC message: '%s' with args '%s'.\n", path, types);

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
	if (result<1) fprintf(stderr, "OSC> ERROR> Error: sending reply failed: %s\n", lo_address_errstr(src));

    return 0;
}


void error_handler(int num, const char *msg, const char *path) {
  fprintf(stderr, "OSC> ERROR> LibLO server error %d in path %s: %s\n", num, path, msg);
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
