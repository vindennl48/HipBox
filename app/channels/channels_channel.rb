class ChannelsChannel < ApplicationCable::Channel
  def subscribed
    # stream_from "some_channel"
    channel = Channel.find(params[:channel_id])
    stream_for channel
  end

  def unsubscribed
    # Any cleanup needed when channel is unsubscribed
  end

  def update(data)
    channel = Channel.find(data['channel_id'])
    ChannelsChannel.broadcast_to(channel, {
      type: "update",
      data: channel
    })
  end

  def saveChannelGain(data)
    channel = Channel.find(data['channel_id'])
    channel.update(gain: data['value'])
  end

  def saveMute(data)
    channel = Channel.find(data['channel_id'])
    channel.update(is_mute: data['value'])
  end

  def saveSolo(data)
    channel = Channel.find(data['channel_id'])
    channel.update(is_solo: data['value'])
  end
end
