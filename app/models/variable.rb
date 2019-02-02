class Variable < ApplicationRecord
  after_save :broadcast_variable

  def self.process_osc(path, value)
    path_split     = path[1..-1].split("/")
    kind name inp  = path_split
    user           = User.find_by(name: name)
    variable       = Variable.find_by(user_id: user.id, name:inp)
    variable.value = value

    self.update_record(variable)
  end

  def self.update_record(variable)
    if variable["kind"] == "mute" and variable["global"]
      self.update_global(variable)
    elsif variable["kind"] == "solo" or variable["kind"] = "record"
      self.update_global(variable)
    else
      record = Variable.find(variable["id"])
      record.update_attributes({ value: variable["value"] })
    end
    puts "variable name: #{variable['name']}, value: #{variable['value']}"
  end

  def self.update_global(variable)
    Variable.where(kind: variable["kind"], name: variable["name"]).each do |v|
      v.update_attributes({ value: variable["value"] })
    end
  end

  private
    def broadcast_variable
      if self.kind != "vol"
        user = User.find(self.user_id)
        UserChannel.broadcast_to(user, { records: [self] })
      end
    end

end
