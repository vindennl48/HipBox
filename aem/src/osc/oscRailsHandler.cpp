#include <string>
#include <lo/lo.h>
#include "../routing/mixer.h"
#include "../data/data.h"
#include "../jack/jack.h"
#include "../extra/json.h"
#include "../extra/print.h"
#include "../extra/find.h"
#include "../extra/db.h"
#include "../extra/getRecNum.h"

#include <typeinfo>

using json = nlohmann::json;
using namespace std;

int rails_reset(const char *path, const char *types, lo_arg **argv, int argc,
		              lo_message msg, void *user_data)
{
  json j3 = json::parse((string)&argv[0]->s);

  if (j3.count("mixers")) {
    // Reset Jack and port objects
    stop_jack();
    reset_data();

    int i_size = j3["mixers"].size();
    for (int i=0; i<i_size; i++) {
      Mixer::create_from_json( &j3["mixers"][i] );
    }

    PRINT("osc.rails_reset> Starting Jack Service");
    start_jack();
    PRINT("osc.rails_reset> Jack Service Up and Running");

    mixer_debug();
    
  } // -- END j3.count("mixers") --

  return 0;
}

int rails_mixer(const char *path, const char *types, lo_arg **argv, int argc,
		              lo_message msg, void *user_data)
{
  json j3 = json::parse((string)&argv[0]->s);

  if (j3.count("mixer")) {
    PRINT("osc.rails_mixer> Mixer data incoming");

    // Incoming channel from OSC
    json *jmixer = &j3["mixer"];

    Mixer *mixer = find_mixer((*jmixer)["id"]);
    if (mixer) {
      if ((*jmixer).count("gain"))
        mixer->set_gain( (*jmixer)["gain"] );

      if ((*jmixer).count("is_recording")) {
        if ((*jmixer)["is_recording"] && !is_recording) {
          is_recording = true;
          PRINT("osc.rails_mixer> Is Recording!");
        } else if (!(*jmixer)["is_recording"] && is_recording){
          is_recording = false;
          PRINT("osc.rails_mixer> Is Not Recording..");
          RecFile::recNum = getRecNum();
        }
      }
    }
  }

  return 0;
}

int rails_channel(const char *path, const char *types, lo_arg **argv, int argc,
		              lo_message msg, void *user_data)
{
  json j3 = json::parse((string)&argv[0]->s);

  if (j3.count("channel")) {
    PRINT("osc.rails_channel> Channel data incoming");

    // Incoming channel from OSC
    json *in_chan_p = &j3["channel"];

    // Current saved channel in mixer
    Channel *channel_p = find_channel((*in_chan_p)["id"]);
    if (channel_p) {
      if ((*in_chan_p).count("gain")) {
        channel_p->set_gain( (*in_chan_p)["gain"] );
        PRINT("osc.rails_channel> Channel Gain DB: %f", slider2db((double)channel_p->gain));
      }
      if ((*in_chan_p).count("pan"))
        channel_p->set_pan( (*in_chan_p)["pan"] );
      if ((*in_chan_p).count("is_mute"))
        channel_p->is_mute = (*in_chan_p)["is_mute"];
    }
  } // -- END j3.count("channels") --

  return 0;
}


