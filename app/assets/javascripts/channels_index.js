function rangeSlider(slider) {
  document.getElementById(slider.dataset.target).innerHTML = slider.value
}

function resetRangeSliderValue(number) {
  number.innerHTML = 50
  document.getElementById(number.dataset.target).value = 50
}

function toggleButton(button) {
  if (button.classList.contains('active')) {
    button.classList.remove('active')
  }
  else {
    button.classList.add('active')
  }
}

function loadActionCable() {
  let sliders = document.querySelectorAll("[data-type=volume-slider]")
  sliders.forEach(function(slider){

    let ID = slider.dataset.channel

    App['channel_'+ID] = App.cable.subscriptions.create({
      channel:    "ChannelsChannel",
      channel_id: ID
    }, {
      connected: function() {
        // Called when the subscription is ready for use on the server
        console.log('Actioncable: Channel #'+ID+' is connected!')
      },

      disconnected: function() {
        // Called when the subscription has been terminated by the server
        console.log('Actioncable: Channel #'+ID+' is disconnected..')
      },

      received: function(data) {
        // Called when there's incoming data on the websocket for this channel
        console.log('Actioncable:')
        console.log(data)
        console.log('------------')
      },

      update: function(value) {
        return this.perform('update', {
          channel_id: ID,
          value:      value
        });
      }
    });

  })
}
