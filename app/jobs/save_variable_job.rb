class SaveVariableJob < ApplicationJob
  queue_as :default

  def perform(variable)
    variable = Variable.find(variable.id)
    variable.update_attributes(variable)
  end
end
