#ifndef HIPBOX_OSC_H
#define HIPBOX_OSC_H

#include <unistd.h>
#include <string>
#include <lo/lo.h>

#define OSC_PORT "4000"

int wildcard_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 lo_message msg, void *user_data);

int ping_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 lo_message msg, void *user_data);

void error_handler(int num, const char *msg, const char *path);

void start_osc();

void stop_osc();

// End OSC Server

#endif
