import Button  from "./button"
import Readout from "./readout"
import Slider  from "./slider"

let Channel_strip = {

  initialize: (ID) => {
    // Create html ID's
    let button_id  = "channel_"+ID+"_button";
    let readout_id = "channel_"+ID+"_readout";
    let slider_id  = "channel_"+ID+"_slider";

    // Get elements
    let button  = document.getElementById(button_id);
    let readout = document.getElementById(readout_id);
    let slider  = document.getElementById(slider_id);

    // Add event listeners for components
    button.addEventListener("click",  function() { Button.toggle_state(button) });
    readout.addEventListener("click", function() { Readout.reset(readout, slider) });
    slider.addEventListener("input",  function() { Slider.on_move(slider, readout) });
  },

  disable: (ID) => {
    // Create html ID's
    let button_id  = "channel_"+ID+"_button";
    let readout_id = "channel_"+ID+"_readout";
    let slider_id  = "channel_"+ID+"_slider";

    // Get elements
    let button  = document.getElementById(button_id);
    let readout = document.getElementById(readout_id);
    let slider  = document.getElementById(slider_id);

    // Hide elements
    button.hidden  = true;
    readout.hidden = true;
    slider.hidden  = true;

    // Remove event listeners from components
    button.removeEventListener("click",  function() { Button.toggle_state(button) });
    readout.removeEventListener("click", function() { Readout.reset(readout, slider) });
    slider.removeEventListener("input",  function() { Slider.on_move(slider, readout) });
  },
  
  set_solo_disable: (ID, state) => {
    // Create html ID's
    let button_id           = "channel_"+ID+"_button";
    let readout_id          = "channel_"+ID+"_readout";
    let readout_disabled_id = "channel_"+ID+"_readout_disabled";
    let slider_id           = "channel_"+ID+"_slider";
    let channel_strip_id    = "channel_"+ID+"_body";
  
    // Get elements
    let button           = document.getElementById(button_id);
    let readout          = document.getElementById(readout_id);
    let readout_disabled = document.getElementById(readout_disabled_id);
    let slider           = document.getElementById(slider_id);
    let channel_strip    = document.getElementById(channel_strip_id);
  
    if (state) {
      readout.hidden          = true;
      readout_disabled.hidden = false;
      slider.hidden           = true;
      button.hidden           = true;
  
      if (!channel_strip.classList.contains("soloed")) {
        channel_strip.classList.add("soloed");
      }
  
    } else {
      readout.hidden          = false;
      readout_disabled.hidden = true;
      slider.hidden           = false;
      button.hidden           = false;
  
      if (channel_strip.classList.contains("soloed")) {
        channel_strip.classList.remove("soloed");
      }
    }
  }

};


export default Channel_strip;
