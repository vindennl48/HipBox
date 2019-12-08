#include "../routing/mixer.h"
#include "../routing/channel.h"
#include "../data/data.h"

Mixer* find_mixer(unsigned int id) {
  int i_size = mixers.size();
  for (int i=0; i<i_size; i++) {
    Mixer *mixer_p = &mixers[i];
    if (mixer_p->id == id) {
      return mixer_p;
    }
  }
  return NULL;
}

Channel* find_channel(unsigned int id) {
  int i_size = mixers.size();
  for (int i=0; i<i_size; i++) {
    Mixer *mixer_p = &mixers[i];

    int j_size = mixer_p->channels.size();
    for (int j=0; j<j_size; j++) {
      Channel *channel_p = &mixer_p->channels[j];
      if (channel_p->id == id) {
        return channel_p;
      }
    }
  }
  return NULL;
}

