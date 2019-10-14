#include <unistd.h>
#include <string>
#include "hipbox.h"

int main (int argc, char *argv[]) {
  // Setup OSC
  start_osc();

  /* keep running until stopped by the user */

  sleep (-1);

  /* this is never reached but if the program
      had some other way to exit besides being killed,
      they would be important to call.
  */

  stop_osc();

  exit (0);
  return 0;
}
