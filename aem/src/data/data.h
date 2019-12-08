#ifndef DATA_H
#define DATA_H

#include <jack/jack.h>
#include <vector>
#include "../routing/inPort.h"
#include "../routing/inPortGroup.h"
#include "../routing/outPortGroup.h"
#include "../routing/channel.h"
#include "../routing/mixer.h"
#include "../audioEngine/audioFile.h"
#include "../audioEngine/audioEngine.h"
#include "../recording/recFile.h"

#define DEFAULT_VECTOR_SIZE  24

extern jack_client_t        *client;
extern vector<InPort>       in_ports;
extern vector<InPortGroup>  in_port_groups;
extern vector<OutPortGroup> out_port_groups;
extern vector<Mixer>        mixers;
extern vector<RecFile>      rec_files;
extern vector<AudioFile>    audio_files;
extern AudioEngine          audio_engine;
extern bool                 is_recording;

void reset_data();

#endif
