#include <lo/lo.h>
#include "osc.h"

void stop_osc() {
  lo_server_thread_stop(server_thread);
  lo_server_thread_free(server_thread);
}

