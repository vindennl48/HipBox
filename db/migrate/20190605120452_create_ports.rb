class CreatePorts < ActiveRecord::Migration[5.2]
  def change
    create_table :ports do |t|
      t.string :name,           null: false, unique: true
      t.boolean :io,            default: true
      t.string :path,           default: ""
      t.decimal :pan,           default: 0
      t.references :port_group, foreign_key: true

      t.timestamps
    end
  end
end
