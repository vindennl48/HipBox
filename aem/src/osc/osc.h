#ifndef START_OSC_H
#define START_OSC_H

#include <lo/lo.h>

#define OSC_PORT "4000"

extern lo_server_thread server_thread;

void start_osc();
void stop_osc();
void error_handler(int num, const char *msg, const char *path);
int wildcard_handler(const char *path, const char *types, lo_arg **argv,
    int argc, lo_message msg, void *user_data);
int rails_reset(const char *path, const char *types, lo_arg **argv,
    int argc, lo_message msg, void *user_data);
int rails_mixer(const char *path, const char *types, lo_arg **argv,
    int argc, lo_message msg, void *user_data);
int rails_channel(const char *path, const char *types, lo_arg **argv,
    int argc, lo_message msg, void *user_data);

#endif
