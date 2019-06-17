#ifndef HIPBOX_JACK_H
#define HIPBOX_JACK_H

#include <jack/jack.h>
#include <lo/lo.h>

// These are based on an 8 input / 10 output audio interface
#define NUM_IN_PORTS          8
#define NUM_IN_PORT_GROUPS   16 
#define NUM_OUT_PORT_GROUPS   5

int rails_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 lo_message msg, void *user_data);

int process(jack_nframes_t nframes, void *arg);

void jack_shutdown(void *arg);

void start_jack();

void stop_jack();

#endif
