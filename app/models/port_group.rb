class PortGroup < ApplicationRecord
  belongs_to     :user, optional: true
  has_many       :ports
  before_destroy :clean_channels
  after_update   :reset_aem
  after_create   :reset_aem
  after_destroy  :reset_aem

  def self.in_port_groups
    PortGroup.where(io: true).includes(:user).order("id")
  end

  def self.out_port_groups
    PortGroup.where(io: false).order("id")
  end

  private

  def clean_channels
    Channel.where(port_group: self).each do |channel|
      channel.destroy
    end
  end

  private

  def reset_aem
    AEM.reset
  end

end
