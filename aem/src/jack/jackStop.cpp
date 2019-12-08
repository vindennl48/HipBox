#include "../data/data.h"

void stop_jack() {
	jack_client_close(client);
}

