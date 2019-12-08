#ifndef CHANNEL_H
#define CHANNEL_H

#include "../routing/inPortGroup.h"

struct Channel {
  InPortGroup *in_port_group;
  unsigned int id      = 0;
  double       gain    = 0.0;
  double       pan     = 0.0;  // -1.0: Left, 1.0: Right
  bool         is_mute = false;
};

#endif
