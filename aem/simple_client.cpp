#include "hipbox_jack.h"
#include "hipbox_osc.h"


// Comment these out to shut them off
/* -------------------------------------------------- */

//#define JACK_AUDIO_SERVER
#define OSC_SERVER

/* -------------------------------------------------- */


int main (int argc, char *argv[]) {

#ifdef JACK_AUDIO_SERVER
  // Setup JACK
  start_jack();
#endif

#ifdef OSC_SERVER
	// Setup OSC
  start_osc();
#endif

	/* keep running until stopped by the user */

	sleep (-1);

	/* this is never reached but if the program
	   had some other way to exit besides being killed,
	   they would be important to call.
	*/

#ifdef OSC_SERVER
  stop_osc();
#endif

#ifdef JACK_AUDIO_SERVER
  stop_jack();
#endif

	exit (0);
}
