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
      data: user
    })
  end

  def saveGain(data)
    user = User.find(data['user_id'])
    user.update(gain: data['value'])
  end
end
