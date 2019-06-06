class CreateChannels < ActiveRecord::Migration[5.2]
  def change
    create_table :channels do |t|
      t.references :user,       foreign_key: true
      t.references :port_group, foreign_key: true
      t.decimal    :gain,       default:     0
      t.decimal    :pan,        default:     0
      t.boolean    :is_mute,    default:     false
      t.boolean    :is_solo,    default:     false

      t.timestamps
    end
  end
end
