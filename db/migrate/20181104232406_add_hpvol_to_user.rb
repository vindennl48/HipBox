class AddHpvolToUser < ActiveRecord::Migration[5.2]
  def change
    add_column :users, :hpVol, :integer, default: 84
  end
end
