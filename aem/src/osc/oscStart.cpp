#include <stdlib.h>  // exit()
#include <lo/lo.h>
#include "osc.h"
#include "../extra/print.h"

pthread_t        dt;
lo_server_thread server_thread;

void start_osc() {
  //start the recording thread
//  pthread_create(&dt, NULL, rec_thread, NULL); 

  server_thread  = NULL;
  lo_server serv = NULL;

  // Create new server
  server_thread = lo_server_thread_new(OSC_PORT, error_handler);
  if (!server_thread) exit(1);

  // Add the methods
  serv = lo_server_thread_get_server( server_thread );
  //lo_server_thread_add_method(server_thread, "/rails", "s", rails_handler, serv);
  lo_server_thread_add_method(server_thread, "/rails/reset",   "s", rails_reset, serv);
  lo_server_thread_add_method(server_thread, "/rails/mixer",   "s", rails_mixer, serv);
  lo_server_thread_add_method(server_thread, "/rails/channel", "s", rails_channel, serv);

  // add method that will match any path and args
  lo_server_thread_add_method(server_thread, NULL, NULL, wildcard_handler, serv);

  // Start the thread
  lo_server_thread_start(server_thread);

  char *url = lo_server_thread_get_url( server_thread );
  PRINT("osc.start_osc> OSC server URL: %s", url);
  free(url);
}

