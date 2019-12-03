import Channels from "../../channels/channels_channel"
import Slider   from "./slider"

let Readout = {

  reset: (readout, slider) => {
    Readout.set_value(readout, 50);
    Slider.set_value(slider, 50);
    Slider.save_value(slider);
  },

  set_value: (readout, value) => {
    readout.innerHTML = value;
  }

};

export default Readout;
