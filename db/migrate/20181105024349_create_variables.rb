class CreateVariables < ActiveRecord::Migration[5.2]
  def change
    create_table :variables do |t|
      t.string :name, null: false
      t.boolean :status, default: false
      t.integer :value, default: 0

      t.timestamps
    end
  end
end
