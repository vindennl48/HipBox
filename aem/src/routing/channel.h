#ifndef CHANNEL_H
#define CHANNEL_H

#include "../routing/inPortGroup.h"
#include "../extra/json.h"

using json = nlohmann::json;

struct Channel {
  InPortGroup *in_port_group;
  unsigned int id      = 0;
  double       gain    = 0.0;
  double       pan     = 0.0;  // -1.0: Left, 1.0: Right
  bool         is_mute = false;

  void set_gain(json new_gain) {
    gain = stod((string)new_gain);
  }

  void set_pan(json new_pan) {
    pan = stod((string)new_pan);
  }

  static Channel create_from_json(json *jchannel);
};

#endif
