#include <unistd.h>        // sleep()
#include <stdlib.h>        // exit()
#include "./src/osc/osc.h" // start_osc(), stop_osc()

int main (int argc, char *argv[]) {
  start_osc();

  sleep (-1);

  stop_osc();

  exit (0);
  return 0;
}
