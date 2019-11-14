class ChangeSamplesToClips < ActiveRecord::Migration[5.2]
  def change
    rename_table :samples, :clips
  end
end
