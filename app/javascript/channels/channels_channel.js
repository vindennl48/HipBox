import Consumer      from "./consumer"
import ChannelStrip  from "../modules/channel_strip/channel_strip"
import Button        from "../modules/channel_strip/button"
import Readout       from "../modules/channel_strip/readout"
import Slider        from "../modules/channel_strip/slider"

let Channels = {

  new_channel_strip: (ID) => {
    Channels["channel_"+ID] = Consumer.subscriptions.create({
      channel:    "ChannelsChannel",
      channel_id: ID
    },{
      connected: function() {
        ChannelStrip.initialize(ID);
        this.update()
        console.log("Actioncable: Channel #"+ID+" is connected!");
      },
      disconnected: function() {
        ChannelStrip.disable(ID);
        console.log("Actioncable: Channel #"+ID+" is disconnected..");
      },
      received: function(data) {
        data = data["channel"];
        
        // Create html ID's
        let button_id  = "channel_"+ID+"_button";
        let readout_id = "channel_"+ID+"_readout";
        let slider_id  = "channel_"+ID+"_slider";
        
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

        if ("is_mute" in data) {
          if (button.dataset.type === "mute") {
            button.hidden = false;
            Button.set_state(button, data["is_mute"])
          }
        }

        if ("is_solo" in data) {
          if (button.dataset.type === "solo") {
            button.hidden = false;
            Button.set_state(button, data["is_solo"]);
          } else if (button.dataset.type === "mute") {
            ChannelStrip.set_solo_disable(ID, data["is_solo"]);
          }
        }

      },

      save_slider_value: function(value) {
        return this.perform("save_slider_value", {channel_id: ID, value: value});
      },

      save_mute_state: function(value) {
        return this.perform("save_mute_state", {channel_id: ID, value: value});
      },

      save_solo_state: function(value) {
        return this.perform("save_solo_state", {channel_id: ID, value: value});
      },

      update: function() {
        return this.perform("update", {channel_id: ID});
      }

    });
  }

};


document.addEventListener("turbolinks:load", function() {
  let channel_strips = document.querySelectorAll("[data-type=channel-strip-body]");

  if (channel_strips) {
    channel_strips.forEach(function(channel_strip) {
      let ID = channel_strip.dataset.id;
      Channels.new_channel_strip(ID);
    });
  }
});


export default Channels;
