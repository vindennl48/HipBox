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

  def saveGain(data)
    user = User.find(data['user_id'])
    user.update(gain: data['value'])
  end

  def record(data)
    User.all.each do |user|
      user.update(is_recording: data['value'])
      puts "MIXERCHANNEL> Record Update: #{data['value']}"
    end
  end

end
