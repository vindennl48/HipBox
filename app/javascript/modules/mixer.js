import App from '../channels/consumer.js'

// -- Range Sliders --
function rangeSlider(slider) {
  document.getElementById(slider.dataset.target).innerHTML = slider.value
  App["channel_"+slider.dataset.channel].saveChannelGain(slider.value)
}

function resetRangeSliderValue(number) {
  number.innerHTML = 50
  document.getElementById(number.dataset.target).value = 50
  App["channel_"+number.dataset.channel].saveChannelGain(number.innerHTML)
}

function toggleButton(button) {
  if (button.classList.contains("active")) {
    button.classList.remove("active")
    if (button.dataset.type === "solo-button")
      App["channel_"+button.dataset.channel].saveSolo(false)
    else if (button.dataset.type === "mute-button")
      App["channel_"+button.dataset.channel].saveMute(false)
    else if (button.classList.contains("record"))
      App["mixer"].record(false)
  }
  else {
    button.classList.add("active")
    if (button.dataset.type === "solo-button")
      App["channel_"+button.dataset.channel].saveSolo(true)
    else if (button.dataset.type === "mute-button")
      App["channel_"+button.dataset.channel].saveMute(true)
    else if (button.classList.contains("record"))
      App["mixer"].record(true)
  }
}

function setButton(button, value) {
  if (value === true) {
    if (!button.classList.contains("active")) {
      button.classList.add("active")
    }
  }
  else {
    if (button.classList.contains("active")) {
      button.classList.remove("active")
    }
  }
}

// For when another user solos themselves, this turns your
//  channel strip blue and removes all elements.
function setChanStrip(chanStrip, value) {
  if (value === true) {
    if (!chanStrip.classList.contains("soloed")) {
      chanStrip.classList.add("soloed")

      chanStrip.querySelectorAll("input, button, div[data-type=volume-value]")
        .forEach(function(elm) { elm.hidden = true })

      chanStrip.querySelectorAll("div[data-type=hidden-label]")
        .forEach(function(div) { div.hidden = false })
    }
  }
  else {
    if (chanStrip.classList.contains("soloed")) {
      chanStrip.classList.remove("soloed")

      chanStrip.querySelectorAll("input, button, div[data-type=volume-value]")
        .forEach(function(elm) { elm.hidden = false })

      chanStrip.querySelectorAll("div[data-type=hidden-label]")
        .forEach(function(div) { div.hidden = true })
    }
  }
}

function loadActionCableSliders() {
  let sliders = document.querySelectorAll("[data-type=volume-slider]")
  sliders.forEach(function(slider){

    let ID = slider.dataset.channel

    App["channel_"+ID] = App.cable.subscriptions.create({
      channel:    "ChannelsChannel",
      channel_id: ID
    }, {
      connected: function() {
        // Called when the subscription is ready for use on the server
        console.log("Actioncable: Channel #"+ID+" is connected!")
        this.update()
      },

      disconnected: function() {
        // Called when the subscription has been terminated by the server
        console.log("Actioncable: Channel #"+ID+" is disconnected..")
        location.reload(true)
      },

      received: function(data) {
        // Called when there"s incoming data on the websocket for this channel
        if (data["type"] === "update") {
          let channel = data["channel"]
          let element = document.querySelectorAll("[data-channel = '"+ID+"']")

          element.forEach(function(a){

            // Slider Values
            if (a.dataset.type === "volume-slider") {
              if ("gain" in channel) {
                a.hidden = false
                a.value  = Math.trunc( channel["gain"] )
              }
            }
            else if (a.dataset.type === "volume-value") {
              if ("gain" in channel) {
                a.hidden    = false
                a.innerHTML = Math.trunc( channel["gain"] )
              }
            }

            // Button Values
            else if (a.dataset.type === "solo-button") {
              if ("is_solo" in channel) {
                a.hidden = false
                setButton(a, channel["is_solo"])
              }
            }
            else if (a.dataset.type === "mute-button") {
              if ("is_mute" in channel) {
                a.hidden = false
                setButton(a, channel["is_mute"])
              }
            }
          })

          // This is now separated since all the elements need to be un-hidden
          //  first before this function can work properly.
          element.forEach(function(a){
            if (a.dataset.type === "channel-strip") {
              if ("is_solo" in channel) { setChanStrip(a, channel["is_solo"]) }
            }
          })

        }
      },

      update: function() {
        return this.perform("update", {channel_id: ID});
      },

      saveChannelGain: function(value) {
        return this.perform("saveChannelGain", {channel_id: ID, value: value});
      },

      saveMute: function(value) {
        return this.perform("saveMute", {channel_id: ID, value: value});
      },

      saveSolo: function(value) {
        return this.perform("saveSolo", {channel_id: ID, value: value});
      }
    });

  })
}

// -- Master Slider -

function rangeSliderMaster(slider) {
  document.getElementById(slider.dataset.target).innerHTML = slider.value
  App["mixer"].saveGain(slider.value)
}

function resetRangeSliderValueMaster(number) {
  number.innerHTML = 50
  document.getElementById(number.dataset.target).value = 50
  App["mixer"].saveGain(number.innerHTML)
}

function loadActionCableSlidersMaster() {
  let sliders = document.querySelectorAll("[data-type=volume-slider-master]")
  sliders.forEach(function(slider){

    let ID = slider.dataset.user

    App["mixer"] = App.cable.subscriptions.create({
      channel: "MixerChannel",
      user_id: ID
    }, {
      connected: function() {
        // Called when the subscription is ready for use on the server
        console.log("Actioncable: User #"+ID+" is connected!")
        this.update()
      },

      disconnected: function() {
        // Called when the subscription has been terminated by the server
        console.log("Actioncable: User #"+ID+" is disconnected..")
        location.reload(true)
      },

      received: function(data) {
        // Called when there"s incoming data on the websocket for this channel
        if (data["type"] === "update") {
          let master = data["user"]
          let element = document.querySelectorAll("[data-user='"+ID+"']")
          element.forEach(function(a){
            if (a.dataset.type === "volume-slider-master") {
              if ("gain" in master) {
                a.hidden = false
                a.value = Math.trunc( master["gain"] )
              }
            }
            else if (a.dataset.type == "volume-value-master") {
              if ("gain" in master) {
                a.hidden    = false
                a.innerHTML = Math.trunc( master["gain"] )
              }
            }
            else if (a.dataset.type == "record-button") {
              if ("is_recording" in master) {
                a.hidden    = false
                setButton(a, master["is_recording"])
              }
            }
          })
        }
      },

      update: function() {
        return this.perform("update", {user_id: ID});
      },

      saveGain: function(value) {
        return this.perform("saveGain", {user_id: ID, value: value});
      },

      record: function(value) {
        return this.perform("record", {user_id: ID, value: value});
      }
    });

  })
}

// --

export {
  loadActionCableSliders,
  loadActionCableSlidersMaster,
  rangeSlider,
  rangeSliderMaster,
  resetRangeSliderValue,
  resetRangeSliderValueMaster,
  toggleButton,
};
