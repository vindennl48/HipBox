class Channel < ApplicationRecord
  belongs_to :user
  belongs_to :port_group

  def self.generate_channels(user)
    PortGroup.all.each do |port_group|
      Channel.find_or_create_by(
        user:       user,
        port_group: port_group,
      )
    end
  end

end
