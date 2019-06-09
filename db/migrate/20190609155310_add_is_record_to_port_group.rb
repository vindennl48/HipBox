class AddIsRecordToPortGroup < ActiveRecord::Migration[5.2]
  def change
    add_column :port_groups, :is_record, :boolean, default: true
  end
end
