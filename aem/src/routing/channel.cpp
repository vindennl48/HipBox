#include "channel.h"
#include "../extra/json.h"

using json = nlohmann::json;

Channel Channel::create_from_json(json *jchannel) {
  Channel channel;
  channel.id            = (*jchannel)["id"];
  channel.gain          = stod((string)(*jchannel)["gain"]);
  channel.pan           = stod((string)(*jchannel)["pan"]);
  channel.is_mute       = (*jchannel)["is_mute"];
  channel.in_port_group = InPortGroup::create_from_json( &(*jchannel)["port_group"] );

  return channel;
}
