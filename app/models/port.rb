class Port < ApplicationRecord
  belongs_to :port_group, optional: true
end
