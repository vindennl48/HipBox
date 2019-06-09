class PortGroup < ApplicationRecord
  belongs_to :user, optional: true
  has_many :ports
  before_destroy :clean_channels

  private

  def clean_channels
    Channel.where(port_group: self).each do |channel|
      channel.destroy
    end
  end

end
