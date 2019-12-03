import Channels from "../../channels/channels_channel"

let Button = {

  get_state: (button) => {
    if (button.classList.contains("active")) {
      return true;
    } else {
      return false;
    }
  },

  toggle_state: (button) => {
    let state = false;

    if (Button.get_state(button)) {
      state = false;
    } else {
      state = true;
    }

    Button.set_state(button, state);
    Button.save_state(button);
  },

  set_state: (button, state) => {
    if (state) {
      if (!Button.get_state(button)) {
        button.classList.add("active");
      }
    } else {
      if (Button.get_state(button)) {
        button.classList.remove("active");
      }
    }
  },

  save_state: (button) => {
    let type  = button.dataset.type;
    let ID    = button.dataset.id;
    let state = Button.get_state(button);

    if (type === "mute") {
      console.log("Saving Mute State: "+state);

      Channels["channel_"+ID].save_mute_state(state);

    } else if (type === "solo") {
      console.log("Saving Solo State: "+state);

      Channels["channel_"+ID].save_solo_state(state);

    } else if (type === "record") {
      console.log("Saving Record State: "+state);

      Channels["channel_"+ID].save_record_state(state);

    }
  }

};

export default Button;
