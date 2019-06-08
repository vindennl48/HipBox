class PortGroup < ApplicationRecord
  belongs_to :user, optional: true
  has_many :ports
end
