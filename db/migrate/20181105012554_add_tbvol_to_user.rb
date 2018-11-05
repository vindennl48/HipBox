class AddTbvolToUser < ActiveRecord::Migration[5.2]
  def change
    add_column :users, :tbVol, :integer, default: 84
  end
end
