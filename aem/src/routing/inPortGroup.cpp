#include "inPortGroup.h"
#include "../extra/json.h"
#include "../extra/templates.h"
#include "../data/data.h"

using json = nlohmann::json;

InPortGroup* InPortGroup::create_from_json(json *jin_port_group) {
  // Create InPortGroup
  InPortGroup in_port_group;
  in_port_group.id        = (*jin_port_group)["id"];
  in_port_group.name      = (*jin_port_group)["name"];
  in_port_group.is_record = (*jin_port_group)["is_record"];

  // Create Ports
  json *jports = &(*jin_port_group)["ports"];
  int i_size = jports->size();
  for (int i=0; i<i_size; i++) {
    in_port_group.ports.push_back(
      InPort::create_from_json( &(*jports)[i] )
    );
  }

  return find_or_create<InPortGroup>(&in_port_groups, &in_port_group);
}
