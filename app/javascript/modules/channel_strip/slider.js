import Channels from "../../channels/channels_channel"
import Readout  from "./readout"

let Slider = {

  on_move: (slider, readout) => {
    let value = slider.value;
    Readout.set_value(readout, value);
    Slider.save_value(slider);
  },

  save_value: (slider) => {
    let ID    = slider.dataset.id;
    let value = slider.value;

    console.log("Saving slider value: "+value);

    Channels["channel_"+ID].save_slider_value(value);
  },

  set_value: (slider, value) => {
    slider.value = value;
  }

};


export default Slider
