class CreateWifis < ActiveRecord::Migration[5.2]
  def change
    create_table :wifis do |t|
      t.string :name
      t.string :network
      t.string :password

      t.timestamps
    end
  end
end
