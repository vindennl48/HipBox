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

  def save_slider_value(data)
    channel = Channel.find(data['channel_id'])
    channel.update(gain: data['value'])
  end

  def save_mute_state(data)
    channel = Channel.find(data['channel_id'])

    if channel.port_group.is_global
      channels = Channel.where(port_group: channel.port_group)

      channels.each do |c|
        c.update(is_mute: data['value'])

        if c != channel
          ChannelsChannel.broadcast_to(c, {
            type:    "update",
            channel: { is_mute: c['is_mute'] }
          })
        end
      end
    else
      channel.update(is_mute: data['value'])
    end
  end

  def save_solo_state(data)
    channel  = Channel.find(data['channel_id'])
    channels = Channel.where(port_group: channel.port_group)

    # Tell everyone you are soloed
    channels.each do |c|
      c.update(is_solo: data['value'])

      if c != channel
        ChannelsChannel.broadcast_to(c, {
          type:    "update",
          channel: { is_solo: c['is_solo'] }
        })
      end
    end

    # Mute other users to you
    channels = Channel.where(user: channel.user)

    channels.each do |c|
      if c.port_group.user != nil and c != channel
        c.update(is_mute: data['value'])
        ChannelsChannel.broadcast_to(c, {
          type:    "update",
          channel: { is_mute: c['is_mute'] }
        })
      end
    end

  end
end
