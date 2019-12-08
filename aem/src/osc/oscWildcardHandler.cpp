#include <lo/lo.h>
#include "../extra/print.h"

int wildcard_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 lo_message msg, void *user_data) {
  PRINT("osc.wildcard_handler> Warning: unhandled OSC message: '%s' with args '%s'.\n", path, types);

  return -1;
}

