class TagClip < ApplicationRecord
  belongs_to :tag
  belongs_to :clip
end
