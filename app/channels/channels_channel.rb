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
      type:      "update",
      channel:   channel,
    })
  end

  def saveChannelGain(data)
    channel = Channel.find(data['channel_id'])
    channel.update(gain: data['value'])
  end

  def saveMute(data)
    channel = Channel.find(data['channel_id'])

    if channel.port_group.is_global
      channels = Channel.where(port_group: channel.port_group)

      channels.each do |c|
        c.update(is_mute: data['value'])
        new_data = { is_mute: c['is_mute'] }

        ChannelsChannel.broadcast_to(c, {
          type:    "update",
          channel: new_data
        })
      end
    else
      channel.update(is_mute: data['value'])
    end
  end

  def saveSolo(data)
    channel  = Channel.find(data['channel_id'])
    channels = Channel.where(port_group: channel.port_group)

    channels.each do |c|
      c.update(is_solo: data['value'])
      new_data = { is_solo: c['is_solo'] }

      ChannelsChannel.broadcast_to(c, {
        type:    "update",
        channel: new_data
      })
    end
  end
end
