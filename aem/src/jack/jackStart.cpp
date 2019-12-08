#include <cstddef>
#include <jack/jack.h>
#include "../data/data.h"
#include "../extra/print.h"
#include "jack.h"

void start_jack() {
  const char     *client_name = "hipbox";
	const char     *server_name = NULL;
	jack_options_t options      = JackNullOption;
	jack_status_t  status;
	
	/* open a client connection to the JACK server */
	client = jack_client_open (client_name, options, &status, server_name);
	if (client == NULL) {
		PRINT("jack.start_jack> jack_client_open() failed, "
			 "status = 0x%2.0x", status);
		if (status & JackServerFailed) {
			PRINT("jack.start_jack> Unable to connect to JACK server");
		}
		exit (1);
	}
	if (status & JackServerStarted) {
		PRINT("jack.start_jack> JACK server started");
	}
	if (status & JackNameNotUnique) {
		client_name = jack_get_client_name(client);
		PRINT("jack.start_jack> unique name `%s' assigned", client_name);
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

	PRINT("jack.start_jack> engine sample rate: %" PRIu32 " ",
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

	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */

	if (jack_activate (client)) {
		PRINT("jack.start_jack> cannot activate client");
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

