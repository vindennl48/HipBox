class AddIsRecordingToUsers < ActiveRecord::Migration[5.2]
  def change
    add_column :users, :is_recording, :boolean, default: false
  end
end
