class Channel < ApplicationRecord
  belongs_to :user
  belongs_to :port_group
end
