#include <jack/jack.h>
#include <vector>
#include "data.h"
#include "../routing/inPort.h"
#include "../routing/inPortGroup.h"
#include "../routing/outPortGroup.h"
#include "../routing/channel.h"
#include "../routing/mixer.h"
#include "../audioEngine/audioFile.h"
#include "../audioEngine/audioEngine.h"
#include "../recording/recFile.h"

jack_client_t        *client;
vector<InPort>       in_ports;
vector<InPortGroup>  in_port_groups;
vector<OutPortGroup> out_port_groups;
vector<Mixer>        mixers;
vector<RecFile>      rec_files;
vector<AudioFile>    audio_files;
AudioEngine          audio_engine;
bool                 is_recording = false;

void reset_data() {
  in_ports.clear();
  in_port_groups.clear();
  out_port_groups.clear();
  mixers.clear();
  rec_files.clear();
  audio_files.clear();

  in_ports.reserve(DEFAULT_VECTOR_SIZE);
  in_port_groups.reserve(DEFAULT_VECTOR_SIZE);
  out_port_groups.reserve(DEFAULT_VECTOR_SIZE);
  mixers.reserve(DEFAULT_VECTOR_SIZE);
  rec_files.reserve(DEFAULT_VECTOR_SIZE);
  audio_files.reserve(DEFAULT_VECTOR_SIZE);
}
