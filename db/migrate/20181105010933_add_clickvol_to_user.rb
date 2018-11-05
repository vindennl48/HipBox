class AddClickvolToUser < ActiveRecord::Migration[5.2]
  def change
    add_column :users, :clickVol, :integer, default: 84
  end
end
