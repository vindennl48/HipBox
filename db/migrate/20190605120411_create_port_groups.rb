class CreatePortGroups < ActiveRecord::Migration[5.2]
  def change
    create_table :port_groups do |t|
      t.string :name,            null: false
      t.boolean :io,             default: true
      t.boolean :is_global,      default: false
      t.boolean :is_global_mute, default: false
      t.references :user,        foreign_key: true

      t.timestamps
    end
  end
end
