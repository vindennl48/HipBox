class PortGroup < ApplicationRecord
  belongs_to :user
  has_many :ports
end
