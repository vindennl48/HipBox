class Port < ApplicationRecord
  belongs_to    :port_group, optional: true
  after_update  :reset_aem
  after_create  :reset_aem
  after_destroy :reset_aem

  def self.in_ports
    Port.where(io: true).includes(:port_group).order("id")
  end

  def self.out_ports
    Port.where(io: false).includes(:port_group).order("id")
  end

  private

  def reset_aem
    AEM.reset
  end

end
