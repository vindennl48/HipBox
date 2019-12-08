#ifndef AEM_JACK_H
#define AEM_JACK_H

#include <jack/jack.h>

int process (jack_nframes_t nframes, void *arg);
void jack_shutdown (void *arg);
void start_jack();
void stop_jack();

#endif
