class Port < ApplicationRecord
  belongs_to   :port_group, optional: true
  after_update :restart_aem

  private

  def restart_aem
    Channel.start_aem
  end

end
