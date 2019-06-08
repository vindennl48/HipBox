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
  }
  else {
    button.classList.add("active")
    if (button.dataset.type === "solo-button")
      App["channel_"+button.dataset.channel].saveSolo(true)
    else if (button.dataset.type === "mute-button")
      App["channel_"+button.dataset.channel].saveMute(true)
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
      },

      received: function(data) {
        // Called when there"s incoming data on the websocket for this channel
        if (data["type"] === "update") {
          data = data["data"]
          let element = document.querySelectorAll("[data-channel='"+ID+"']")
          element.forEach(function(a){
            if (a.dataset.type === "volume-slider") {
              a.value = Math.trunc( data["gain"] )
            }
            else if (a.dataset.type === "volume-value") {
              a.innerHTML = Math.trunc( data["gain"] )
            }
            else if (a.dataset.type === "solo-button") {
              if (data["is_solo"] === true) {
                toggleButton(a)
              }
            }
            else if (a.dataset.type === "mute-button") {
              if (data["is_mute"] === true) {
                toggleButton(a)
              }
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
      },

      received: function(data) {
        // Called when there"s incoming data on the websocket for this channel
        if (data["type"] === "update") {
          data = data["data"]
          let element = document.querySelectorAll("[data-user='"+ID+"']")
          element.forEach(function(a){
            if (a.dataset.type === "volume-slider-master")
              a.value = Math.trunc( data["gain"] )
            else
              a.innerHTML = Math.trunc( data["gain"] )
          })
        }
      },

      update: function() {
        return this.perform("update", {user_id: ID});
      },

      saveGain: function(value) {
        return this.perform("saveGain", {user_id: ID, value: value});
      }
    });

  })
}

// --
