class RemoveStatusFromVariables < ActiveRecord::Migration[5.2]
  def change
    remove_column :variables, :status, :boolean
  end
end
