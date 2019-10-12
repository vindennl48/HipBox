#ifndef HIPBOX_H
#define HIPBOX_H

// Set DEBUG to 1 to turn on debug information
#define DEBUG 1
// Macro for printing debug information
#define PRINTD(...) do { if (DEBUG) printf(__VA_ARGS__); } while (0)

#include <lo/lo.h>
#include <jack/jack.h>

#define DEFAULT_VECTOR_SIZE  24
#define OSC_PORT             "4000"
#define RING_BUFF_SIZE       (44100 * 10)

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
