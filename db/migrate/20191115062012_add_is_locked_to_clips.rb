class AddIsLockedToClips < ActiveRecord::Migration[5.2]
  def change
    add_column :clips, :is_locked, :boolean
  end
end
