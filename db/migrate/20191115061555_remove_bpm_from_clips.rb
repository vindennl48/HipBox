class RemoveBpmFromClips < ActiveRecord::Migration[5.2]
  def change
    remove_column :clips, :bpm, :integer
  end
end
