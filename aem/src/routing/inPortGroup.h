#ifndef INPORTGROUP_H
#define INPORTGROUP_H

#include <vector>
#include <string>
#include "../routing/inPort.h"

using namespace std;

struct InPortGroup {
  unsigned int    id        = 0;
  string          name      = "";
  bool            is_record = false;
  vector<InPort*> ports;

  void initialize(jack_client_t *client) {
    int size = ports.size();
    for (int i=0; i<size; i++) {
      ports[i]->initialize(client);
    }
  }

  void connect(jack_client_t *client) {
    int size = ports.size();
    for (int i=0; i<size; i++) {
      ports[i]->connect(client);
    }
  }

};

#endif
