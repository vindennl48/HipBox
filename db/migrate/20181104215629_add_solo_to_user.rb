class AddSoloToUser < ActiveRecord::Migration[5.2]
  def change
    add_column :users, :solo, :boolean, default: false
  end
end
