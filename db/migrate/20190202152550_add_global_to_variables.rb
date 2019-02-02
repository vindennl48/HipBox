class AddGlobalToVariables < ActiveRecord::Migration[5.2]
  def change
    add_column :variables, :global, :boolean, default: false
  end
end
