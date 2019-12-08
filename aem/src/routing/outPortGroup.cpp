#include "outPortGroup.h"
#include "../extra/json.h"
#include "../extra/templates.h"
#include "../data/data.h"

using json = nlohmann::json;

OutPortGroup* OutPortGroup::create_from_json(json *jout_port_group) {
  OutPortGroup out_port_group;
  out_port_group.id                       = (*jout_port_group)["id"];
  out_port_group.name                     = (*jout_port_group)["name"];
  out_port_group.port_name_left           = (*jout_port_group)["ports"][0]["name"];
  out_port_group.port_name_right          = (*jout_port_group)["ports"][1]["name"];
  out_port_group.hardware_port_path_left  = (*jout_port_group)["ports"][0]["path"];
  out_port_group.hardware_port_path_right = (*jout_port_group)["ports"][1]["path"];

  return find_or_create<OutPortGroup>(&out_port_groups, &out_port_group);
}
