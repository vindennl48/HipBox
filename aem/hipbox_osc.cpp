#include <iostream>
#include <string>
#include "hipbox_osc.h"
#include "hipbox_jack.h"

lo_server_thread server_thread = NULL;

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


// OSC Server

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

// End OSC Server
