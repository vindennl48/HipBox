class MixerChannel < ApplicationCable::Channel
  def subscribed
    # stream_from "some_channel"
    user = User.find(params[:user_id])
    stream_for user
  end

  def unsubscribed
    # Any cleanup needed when channel is unsubscribed
  end

  def update(data)
    user = User.find(data['user_id'])
    MixerChannel.broadcast_to(user, {
      type: "update",
      user: user
    })
  end

  def save_slider_value(data)
    user = User.find(data['user_id'])
    user.update(gain: data['value'])
  end

  def save_record_state(data)
    User.all.each do |user|
      user.update(is_recording: data['value'])
      MixerChannel.broadcast_to(user, {
        type: "update",
        user: user,
      })
      puts "MIXERCHANNEL> Record Update: #{data['value']}"
      $BLOCKUSEROSC = true
    end
    $BLOCKUSEROSC = false
  end

end
