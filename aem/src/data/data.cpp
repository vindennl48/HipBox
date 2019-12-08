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

//static time_t now  = time(0);
//static tm     *ltm = localtime(&now);

jack_client_t        *client;
vector<InPort>       in_ports;
vector<InPortGroup>  in_port_groups;
vector<OutPortGroup> out_port_groups;
vector<Mixer>        mixers;
vector<RecFile>      rec_files;
vector<AudioFile>    audio_files;
AudioEngine          audio_engine;
bool                 is_recording = false;
