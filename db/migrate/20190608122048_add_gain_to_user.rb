class AddGainToUser < ActiveRecord::Migration[5.2]
  def change
    add_column :users, :gain, :decimal, default: 10
  end
end
