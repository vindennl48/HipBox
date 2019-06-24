#ifndef HIPBOX_H
#define HIPBOX_H

#include <lo/lo.h>
#include <jack/jack.h>

// These are based on an 8 input / 10 output audio interface
#define NUM_IN_PORTS        16
#define NUM_IN_PORT_GROUPS  16
#define NUM_OUT_PORT_GROUPS 5
#define NUM_MIXER_CHANNELS  16 
#define OSC_PORT            "4000"

// -- OSC --
int  wildcard_handler(const char *path, const char *types, lo_arg **argv, int argc,
                		  lo_message msg, void *user_data);
int  ping_handler(const char *path, const char *types, lo_arg **argv, int argc,
            		  lo_message msg, void *user_data);
void error_handler(int num, const char *msg, const char *path);
void start_osc();
void stop_osc();
// --

// -- JACK --
void jack_shutdown(void *arg);
int  process(jack_nframes_t nframes, void *arg);
int  rails_handler(const char *path, const char *types, lo_arg **argv, int argc,
                   lo_message msg, void *user_data);
void start_jack();
void stop_jack();
// --

#endif
