class CreatePorts < ActiveRecord::Migration[5.2]
  def change
    create_table :ports do |t|
      t.string :name
      t.boolean :io
      t.string :path
      t.decimal :pan
      t.references :port_group, foreign_key: true

      t.timestamps
    end
  end
end
