#ifndef HIPBOX_JACK_H
#define HIPBOX_JACK_H

#include <jack/jack.h>

// These are based on an 8 input / 10 output audio interface
#define NUM_IN_PORTS         8
#define NUM_IN_PORT_GROUPS   8
#define NUM_OUT_PORT_GROUPS  5

int process (jack_nframes_t nframes, void *arg);

void jack_shutdown (void *arg);

void start_jack();

void stop_jack();

#endif
