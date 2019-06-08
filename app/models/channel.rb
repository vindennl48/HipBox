class Channel < ApplicationRecord
  belongs_to    :user
  belongs_to    :port_group
  # before_update :updateActionCable

  def self.generate_channels(user)
    PortGroup.all.order("id").each do |port_group|
      Channel.find_or_create_by(
        user:       user,
        port_group: port_group,
      )
    end
  end

  # private

  # def updateActionCable
  #   ChannelsChannel.broadcast_to(self, {type: 'update', data: 'hey'})
  # end

end
