class Port < ApplicationRecord
  belongs_to    :port_group, optional: true
  after_update  :restart_aem
  after_create  :restart_aem
  after_destroy :restart_aem

  private

  def restart_aem
    AEM.init
  end

end
