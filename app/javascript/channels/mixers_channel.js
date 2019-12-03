import Consumer     from "./consumer"
import Channels     from "./channels_channel"
import ChannelStrip from "../modules/channel_strip/channel_strip"
import Button       from "../modules/channel_strip/button"
import Readout      from "../modules/channel_strip/readout"
import Slider       from "../modules/channel_strip/slider"

let MasterBus = {

  new_master_bus: (ID) => {
    Channels["channel_0"] = Consumer.subscriptions.create({
      channel: "MixerChannel",
      user_id: ID
    },{
      connected: function() {
        ChannelStrip.initialize(0);
        this.update()
        console.log("Actioncable: Master Bus is connected!");
      },
      disconnected: function() {
        ChannelStrip.disable(0);
        console.log("Actioncable: Master Bus is disconnected..");
      },
      received: function(data) {
        data = data["user"];
        
        // Create html ID's
        let button_id  = "channel_0_button";
        let readout_id = "channel_0_readout";
        let slider_id  = "channel_0_slider";
        
        // Get elements
        let button  = document.getElementById(button_id);
        let readout = document.getElementById(readout_id);
        let slider  = document.getElementById(slider_id);
        
        if ("gain" in data) {
          // make sure elements are unhidden
          slider.hidden  = false;
          readout.hidden = false;
    
          Slider.set_value(slider, Math.trunc(data["gain"]));
          Readout.set_value(readout, Math.trunc(data["gain"]));
        }
    
        if ("is_recording" in data) {
          if (button.dataset.type === "record") {
            button.hidden = false;
            Button.set_state(button, data["is_recording"])
          }
        }
    
      },
    
      save_slider_value: function(value) {
        return this.perform("save_slider_value", {user_id: ID, value: value});
      },
    
      save_record_state: function(value) {
        return this.perform("save_record_state", {user_id: ID, value: value});
      },
    
      update: function() {
        return this.perform("update", {user_id: ID});
      }
    
    });
  }

};


document.addEventListener("turbolinks:load", function() {
  let master_bus = document.getElementById("channel_0_body");

  if (master_bus) {
    let ID = master_bus.dataset.userid;
    MasterBus.new_master_bus(ID);
  }
});


export default MasterBus;
