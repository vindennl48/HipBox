class Port < ApplicationRecord
  belongs_to    :port_group, optional: true
  after_update  :reset_aem
  after_create  :reset_aem
  after_destroy :reset_aem

  private

  def reset_aem
    AEM.reset
  end

end
