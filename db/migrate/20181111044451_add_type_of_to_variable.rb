class AddTypeOfToVariable < ActiveRecord::Migration[5.2]
  def change
    add_column :variables, :type_of, :string
  end
end
