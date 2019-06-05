class CreateChannels < ActiveRecord::Migration[5.2]
  def change
    create_table :channels do |t|
      t.references :user, foreign_key: true
      t.references :port_group, foreign_key: true
      t.decimal :gain
      t.decimal :pan
      t.boolean :is_mute
      t.boolean :is_solo

      t.timestamps
    end
  end
end
