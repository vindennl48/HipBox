class CreateClicks < ActiveRecord::Migration[5.2]
  def change
    create_table :clicks do |t|
      t.string :name
      t.string :path, default: ""
      t.decimal :gain, default: 0
      t.boolean :is_high, default: false
      t.boolean :is_active, default: false

      t.timestamps
    end
  end
end
