class AddGainToUser < ActiveRecord::Migration[5.2]
  def change
    add_column :users, :gain, :decimal, default: 0
  end
end
