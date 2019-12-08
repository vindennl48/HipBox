#ifndef MIXER_H
#define MIXER_H

#include <vector>
#include "../routing/channel.h"
#include "../routing/outPortGroup.h"
#include "../extra/json.h"

using json = nlohmann::json;
using namespace std;

struct Mixer {
  vector<Channel> channels;
  OutPortGroup    *out_port_group;
  unsigned int    id   = 0;
  double          gain = 0.0;

  void set_gain(json new_gain) {
    gain = stod((string)new_gain);
  }

  static Mixer* create_from_json(json *jmixer);
};

void mixer_debug();

#endif
