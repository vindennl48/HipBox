// App.channels = App.cable.subscriptions.create("ChannelsChannel", {
//   connected: function() {
//     // Called when the subscription is ready for use on the server
//     console.log('Actioncable: Channels channel is connected!')
//   },
// 
//   disconnected: function() {
//     // Called when the subscription has been terminated by the server
//     console.log('Actioncable: Channels channel is disconnected...')
//   },
// 
//   received: function(data) {
//     // Called when there's incoming data on the websocket for this channel
//     console.log('Actioncable:')
//     console.log(data)
//     console.log('------------')
//   },
// 
//   update: function() {
//     return this.perform('update');
//   }
// });
