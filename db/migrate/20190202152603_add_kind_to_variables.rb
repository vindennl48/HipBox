class AddKindToVariables < ActiveRecord::Migration[5.2]
  def change
    add_column :variables, :kind, :string, null: false
  end
end
