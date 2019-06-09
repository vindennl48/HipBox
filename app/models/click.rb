class Click < ApplicationRecord
  before_update :set_active_click

  private
  
  def set_active_click
    if self.is_active
      Click.where(is_active: true, is_high: self.is_high).each do |click|
        if click != self
          click.update(is_active: false)
        end
      end
    end
  end

end
