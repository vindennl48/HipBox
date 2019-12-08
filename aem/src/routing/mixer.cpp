#include <vector>
#include <string>
#include "mixer.h"
#include "../routing/channel.h"
#include "../routing/outPortGroup.h"
#include "../extra/json.h"
#include "../extra/templates.h"
#include "../data/data.h"

using json = nlohmann::json;
using namespace std;

Mixer* Mixer::create_from_json(json *jmixer) {
  // Create mixer and outPortGroup
  Mixer mixer;
  mixer.id             = (*jmixer)["id"];
  mixer.gain           = stod((string)(*jmixer)["gain"]);
  mixer.out_port_group = OutPortGroup::create_from_json( &(*jmixer)["out_port_group"] );

  // Create channels
  json *jchannels = &(*jmixer)["channels"];
  int i_size      = jchannels->size();

  for (int i=0; i<i_size; i++) {
    mixer.channels.push_back(
      Channel::create_from_json( &(*jchannels)[i] )
    );
  }

  return find_or_create<Mixer>(&mixers, &mixer);
}

void mixer_debug() {
  //* All the testing to confirm this works
  
  // Test
  int i_size = in_port_groups.size();
  PRINT("routing.mixer_debug> InPortGroups Size: %i", i_size);
  for (int i=0; i<i_size; i++) {
    PRINT("routing.mixer_debug> InPortGroup Name: %s", in_port_groups[i].name.c_str());
  }
  
  
  // Test
  i_size = in_ports.size();
  PRINT("routing.mixer_debug> InPorts Size: %i", i_size);
  for (int i=0; i<i_size; i++) {
    PRINT("routing.mixer_debug> InPort Name: %s, Path: %s",
      in_ports[i].name.c_str(),
      in_ports[i].hardware_port_path.c_str()
    );
  }
  
  // Test for port groups
  i_size = in_port_groups.size();
  PRINT("routing.mixer_debug> InPortGroups Size: %i", i_size);
  for (int i=0; i<i_size; i++) {
    PRINT("routing.mixer_debug> InPortGroup Ports Size: %i", (int)in_port_groups[i].ports.size());
      if (in_port_groups[i].ports.size() > 0) {
      PRINT("routing.mixer_debug> InPortGroup FirstPortName: %s", in_port_groups[i].ports[0]->hardware_port_path.c_str());
      }
  }
  
  // Test
  i_size = out_port_groups.size();
  PRINT("routing.mixer_debug> OutPortGroups Size: %i", i_size);
  for (int i=0; i<i_size; i++) {
    PRINT("routing.mixer_debug> OutPortGroup LeftName: %s, RightName: %s, LeftPath: %s, RightPath: %s",
      out_port_groups[i].port_name_left.c_str(),
      out_port_groups[i].port_name_right.c_str(),
      out_port_groups[i].hardware_port_path_left.c_str(),
      out_port_groups[i].hardware_port_path_right.c_str()
    );
  }
  //*/
}

