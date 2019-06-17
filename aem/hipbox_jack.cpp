#include <vector>
#include <jack/jack.h>
#include <lo/lo.h>
#include "json.h"
#include "db.h"
#include "structures.h"
#include "hipbox_jack.h"

using json = nlohmann::json;

jack_client_t *client;
InPort        in_ports[NUM_IN_PORTS];
InPortGroup   in_port_groups[NUM_IN_PORT_GROUPS];
OutPortGroup  out_port_groups[NUM_OUT_PORT_GROUPS];
Mixer         mixers[NUM_OUT_PORT_GROUPS];

int rails_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 lo_message msg, void *user_data)
{
  auto j3 = json::parse((std::string)&argv[0]->s);
  printf("----> rails_handler\n");
  //printf("----> Json: %s \n", j3.dump().c_str());
  //printf("----> %s \n", j3["inPorts"].dump().c_str());

  /* Updates that require jack restart */
  if (j3.count("inPorts")) {
    int size = j3["inPorts"].size();
    printf("----> Size: %i \n", size);
    for (int i=0; i<size; i++) {
      in_ports[i].port_name = (std::string)j3["inPorts"][i]["name"];
      printf("----> InPorts %i - Name: %s \n", i, in_ports[i].port_name.c_str());
      //std::cout << "----> InPorts: " << (std::string)j3["inPorts"][i]["name"] << std::endl;
    }
  }
  /* END: Updates that require jack restart */

  return 0;
}

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
