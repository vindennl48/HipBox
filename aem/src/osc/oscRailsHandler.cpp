#include <string>
#include <lo/lo.h>
#include "../extra/json.h"
#include "../extra/print.h"
#include "../extra/find.h"
#include "../extra/templates.h"
#include "../extra/db.h"
#include "../extra/getRecNum.h"
#include "../data/data.h"
#include "../jack/jack.h"

using json = nlohmann::json;
using namespace std;

int rails_handler(const char *path, const char *types, lo_arg **argv, int argc,
		              lo_message msg, void *user_data)
{
  auto j3 = json::parse((string)&argv[0]->s);

  PRINT("osc.rails_handler> Starting Rails Handler");

  /* Dump entire json string */
  //PRINT("osc.rails_handler> %s", j3.dump(4).c_str());

  if (j3.count("mixers")) {
    auto *mixers_p = &j3["mixers"];

    /* Reset */
    stop_jack();
    in_ports.clear();
    in_port_groups.clear();
    out_port_groups.clear();
    mixers.clear();
    rec_files.clear();
    audio_files.clear();

    in_ports.reserve(DEFAULT_VECTOR_SIZE);
    in_port_groups.reserve(DEFAULT_VECTOR_SIZE);
    out_port_groups.reserve(DEFAULT_VECTOR_SIZE);
    mixers.reserve(DEFAULT_VECTOR_SIZE);
    rec_files.reserve(DEFAULT_VECTOR_SIZE);
    audio_files.reserve(DEFAULT_VECTOR_SIZE);
    /* -- */

    int i_size = mixers_p->size();
    for (int i=0; i<i_size; i++) {
      auto *mixer_p          = &(*mixers_p)[i];
      auto *out_port_group_p = &(*mixers_p)[i]["out_port_group"];
      auto *channels_p       = &(*mixers_p)[i]["channels"];

      OutPortGroup out_port_group;
      out_port_group.id                       = (*out_port_group_p)["id"];
      out_port_group.name                     = (*out_port_group_p)["name"];
      out_port_group.port_name_left           = (*out_port_group_p)["ports"][0]["name"];
      out_port_group.port_name_right          = (*out_port_group_p)["ports"][1]["name"];
      out_port_group.hardware_port_path_left  = (*out_port_group_p)["ports"][0]["path"];
      out_port_group.hardware_port_path_right = (*out_port_group_p)["ports"][1]["path"];

      Mixer mixer;
      mixer.id             = (*mixer_p)["id"];
      mixer.gain           = stod((string)(*mixer_p)["gain"]);
      mixer.out_port_group = find_or_create<OutPortGroup>(&out_port_groups, &out_port_group);

      int j_size = channels_p->size();
      for (int j=0; j<j_size; j++) {
        auto *channel_p = &(*channels_p)[j];

        InPortGroup in_port_group;
        in_port_group.id        = (*channel_p)["port_group"]["id"];
        in_port_group.name      = (*channel_p)["port_group"]["name"];
        in_port_group.is_record = (*channel_p)["port_group"]["is_record"];

        int k_size = (*channel_p)["port_group"]["ports"].size();
        for (int k=0; k<k_size; k++) {
          auto *port_p = &(*channel_p)["port_group"]["ports"][k];

          InPort in_port;
          in_port.id                 = (*port_p)["id"];
          in_port.pan                = stod((string)(*port_p)["pan"]);
          in_port.name               = (*port_p)["name"];
          in_port.hardware_port_path = (*port_p)["path"];

          in_port_group.ports.push_back(
            find_or_create<InPort>(&in_ports, &in_port)
          );
        }

        // Create recfile for recordable in_port_groups
        if (in_port_group.is_record) {
          RecFile rec_file;
          rec_file.id        = in_port_group.id;
          rec_file.base_name = in_port_group.name;

          rec_file.in_ports_p.insert(
            rec_file.in_ports_p.end(),
            in_port_group.ports.begin(),
            in_port_group.ports.end()
          );

          find_or_create<RecFile>(&rec_files, &rec_file);
        }

        Channel channel;
        channel.id            = (*channel_p)["id"];
        channel.gain          = stod((string)(*channel_p)["gain"]);
        channel.pan           = stod((string)(*channel_p)["pan"]);
        channel.is_mute       = (*channel_p)["is_mute"];
        channel.in_port_group = find_or_create<InPortGroup>(&in_port_groups, &in_port_group);

        mixer.channels.push_back(channel);
      }

      mixers.push_back(mixer);
    }

    // Create recfile for scratch track
    RecFile rec_file;
    rec_file.id        = 9999;
    rec_file.base_name = "Scratch";

    i_size = in_port_groups.size();
    for (int i=0; i<i_size; i++) {
      InPortGroup *in_port_group = &in_port_groups[i];

      if (in_port_group->is_record) {
        rec_file.in_ports_p.insert(
          rec_file.in_ports_p.end(),
          in_port_group->ports.begin(),
          in_port_group->ports.end()
        );
      }
    }
    find_or_create<RecFile>(&rec_files, &rec_file);
    // Done creating scratch track

    PRINT("osc.rails_handler> Starting Jack Service\n");
    start_jack();
    PRINT("osc.rails_handler> Jack Service Up and Running\n");

#ifdef DEBUG
   //* All the testing to confirm this works
    
    // Test
    i_size = in_port_groups.size();
    PRINT("osc.rails_handler> InPortGroups Size: %i\n", i_size);
    for (int i=0; i<i_size; i++) {
      PRINT("osc.rails_handler> InPortGroup Name: %s\n", in_port_groups[i].name.c_str());
    }
    
    
    // Test
    i_size = in_ports.size();
    PRINT("osc.rails_handler> InPorts Size: %i\n", i_size);
    for (int i=0; i<i_size; i++) {
      PRINT("osc.rails_handler> InPort Name: %s, Path: %s\n",
        in_ports[i].name.c_str(),
        in_ports[i].hardware_port_path.c_str()
      );
    }
    
    // Test for port groups
    i_size = in_port_groups.size();
    PRINT("osc.rails_handler> InPortGroups Size: %i\n", i_size);
    for (int i=0; i<i_size; i++) {
      PRINT("osc.rails_handler> InPortGroup Ports Size: %i\n", (int)in_port_groups[i].ports.size());
       if (in_port_groups[i].ports.size() > 0) {
        PRINT("osc.rails_handler> InPortGroup FirstPortName: %s\n", in_port_groups[i].ports[0]->hardware_port_path.c_str());
       }
    }
    
    // Test
    i_size = out_port_groups.size();
    PRINT("osc.rails_handler> OutPortGroups Size: %i\n", i_size);
    for (int i=0; i<i_size; i++) {
      PRINT("osc.rails_handler> OutPortGroup LeftName: %s, RightName: %s, LeftPath: %s, RightPath: %s\n",
        out_port_groups[i].port_name_left.c_str(),
        out_port_groups[i].port_name_right.c_str(),
        out_port_groups[i].hardware_port_path_left.c_str(),
        out_port_groups[i].hardware_port_path_right.c_str()
      );
    }
    //*/
#endif
    
  } // -- END j3.count("mixers") --

  if (j3.count("mixer")) {
    PRINT("osc.rails_handler> Mixer data incoming\n");

    // Incoming channel from OSC
    auto *in_mixer_p = &j3["mixer"];

    Mixer *mixer_p = find_mixer((*in_mixer_p)["id"]);
    if (mixer_p) {
      if ((*in_mixer_p).count("gain"))
        mixer_p->gain = stod((string)(*in_mixer_p)["gain"]);

      if ((*in_mixer_p).count("is_recording")) {
        if ((*in_mixer_p)["is_recording"] && !is_recording) {
          is_recording = true;
          PRINT("osc.rails_handler> Is Recording!\n");
        } else if (!(*in_mixer_p)["is_recording"] && is_recording){
          is_recording = false;
          PRINT("osc.rails_handler> Is Not Recording..\n");
          RecFile::recNum = getRecNum();
        }
      }
    }
  }

  if (j3.count("channel")) {
    PRINT("osc.rails_handler> Channel data incoming\n");

    // Incoming channel from OSC
    auto *in_chan_p = &j3["channel"];

    // Current saved channel in mixer
    Channel *channel_p = find_channel((*in_chan_p)["id"]);
    if (channel_p) {
      if ((*in_chan_p).count("gain")) {
        channel_p->gain = stod((string)(*in_chan_p)["gain"]);
        PRINT("osc.rails_handler> Channel Gain DB: %f\n", slider2db((double)channel_p->gain));
      }
      if ((*in_chan_p).count("pan"))
        channel_p->pan = stod((string)(*in_chan_p)["pan"]);
      if ((*in_chan_p).count("is_mute"))
        channel_p->is_mute = (*in_chan_p)["is_mute"];
    }
  } // -- END j3.count("channels") --

  return 0;
}
