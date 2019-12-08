#include "inPort.h"
#include "../extra/json.h"
#include "../extra/templates.h"
#include "../data/data.h"

using json = nlohmann::json;

InPort* InPort::create_from_json(json *jin_port) {
  InPort in_port;
  in_port.id                 = (*jin_port)["id"];
  in_port.pan                = stod((string)(*jin_port)["pan"]);
  in_port.name               = (*jin_port)["name"];
  in_port.hardware_port_path = (*jin_port)["path"];

  return find_or_create<InPort>(&in_ports, &in_port);
}
