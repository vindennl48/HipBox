#include "../extra/print.h"

void error_handler(int num, const char *msg, const char *path) {
  PRINT("osc.error_handler> LibLO server error %d in path %s: %s", num, path, msg);
  fflush(stdout);
}

