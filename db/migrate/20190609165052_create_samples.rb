class CreateSamples < ActiveRecord::Migration[5.2]
  def change
    create_table :samples do |t|
      t.string :name
      t.string :path, default: ""
      t.decimal :gain, default: 0
      t.bigint :start, default: 0
      t.bigint :end, default: 0
      t.integer :bpm, default: 120

      t.timestamps
    end
  end
end
