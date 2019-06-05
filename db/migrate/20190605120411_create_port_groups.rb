class CreatePortGroups < ActiveRecord::Migration[5.2]
  def change
    create_table :port_groups do |t|
      t.string :name
      t.boolean :io
      t.boolean :is_global
      t.boolean :is_global_mute
      t.references :user, foreign_key: true

      t.timestamps
    end
  end
end
