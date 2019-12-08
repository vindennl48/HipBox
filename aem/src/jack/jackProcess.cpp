#include <jack/jack.h>
#include "../data/data.h"
#include "../extra/db.h"

int process (jack_nframes_t nframes, void *arg) {

  /* Initialize in_ports */
  int i_size = in_ports.size();
  for (int i=0; i<i_size; i++)
    in_ports[i].initPort(nframes);

  /* Loop through all Mixers */
  i_size = mixers.size();
  for (int i=0; i<i_size; i++) {
    Mixer        *mixer          = &mixers[i];
    OutPortGroup *out_port_group = mixer->out_port_group;

    out_port_group->initPort(nframes);

    /* Loop through all Channels in Mixer */
    int j_size = mixer->channels.size();
    for (int j=0; j<j_size; j++) {
      Channel     *channel       = &mixer->channels[j];
      InPortGroup *in_port_group = channel->in_port_group;

      if (!channel->is_mute) {

        /* Loop through all InPorts in Channel */
        int k_size = in_port_group->ports.size();
        for (int k=0; k<k_size; k++) {
          if (in_port_group->ports[k]->checkRealPort()) {
            InPort *in_port = in_port_group->ports[k];
            //in_port->initPort(nframes);

            double in_port_pan_left = 1, in_port_pan_right = 1;
            if (in_port->pan < 0)
              in_port_pan_right += in_port->pan;
            else
              in_port_pan_left -= in_port->pan;

            double channel_pan_left = 1, channel_pan_right = 1;
            if (channel->pan < 0)
              channel_pan_right += channel->pan;
            else
              channel_pan_left -= channel->pan;

            for (int l=0; l<nframes; l++) {
                                              //    raw audio           in port pan          channel pan          channel gain                 mixer gain
              out_port_group->output_left[l]  += (((in_port->input[l] * in_port_pan_left)  * channel_pan_left)  * slider2lin(channel->gain)) * slider2lin(mixer->gain);
              out_port_group->output_right[l] += (((in_port->input[l] * in_port_pan_right) * channel_pan_right) * slider2lin(channel->gain)) * slider2lin(mixer->gain);
            }
          }
        }

      }
    }
  }

  // process recordings
//  i_size = rec_files.size();
//  for (int i=0; i<i_size; i++)
//    rec_files[i].set_next_jack_buff(nframes);

	return 0;
} // -- END Process --


