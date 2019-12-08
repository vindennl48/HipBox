#ifndef MIXER_H
#define MIXER_H

#include <vector>
#include "../routing/channel.h"
#include "../routing/outPortGroup.h"

using namespace std;

struct Mixer {
  vector<Channel> channels;
  OutPortGroup    *out_port_group;
  unsigned int    id   = 0;
  double          gain = 0.0;
};

#endif
