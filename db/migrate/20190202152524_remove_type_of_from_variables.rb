class RemoveTypeOfFromVariables < ActiveRecord::Migration[5.2]
  def change
    remove_column :variables, :type_of, :string
  end
end
