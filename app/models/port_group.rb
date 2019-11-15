class PortGroup < ApplicationRecord
  belongs_to     :user, optional: true
  has_many       :ports
  before_destroy :clean_channels
  after_update   :restart_aem
  after_create   :restart_aem
  after_destroy  :restart_aem

  private

  def clean_channels
    Channel.where(port_group: self).each do |channel|
      channel.destroy
    end
  end

  private

  def restart_aem
    AEM.init
  end

end
