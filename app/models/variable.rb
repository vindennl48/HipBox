class Variable < ApplicationRecord
  after_save :broadcast_variable

  def self.process_osc(path, value)
    path_sp = path[1..-1].split("/")

    if path_sp[0] != "rails"
      return 0
    else
      path_sp.shift(1)
    end

    # issue commands to rails via OSC here
    if path_sp[0] == "maintenance"
      self.maintenance(path_sp, value)
      return 1
    end

    # process incoming OSC variable
    if path_sp.size == 3
      kind, name, inp = path_sp
      toggle = false
    elsif path_sp.size == 4
      kind, name, inp, toggle = path_sp
    end

    user     = User.find_by(name: name)
    variable = Variable.find_by(user_id: user.id, name: inp, kind: kind)

    if toggle
      variable.value = variable.value == 0 ? 1 : 0
    else
      variable.value = value
    end

    self.update_record(variable)
  end

  def self.maintenance(path_sp, value)
    kind, action = path_sp

    if action == "dump_vars"
      self.dump_vars
    end

  end

  def self.dump_vars
    # This is how the mixers load user presets on system start.
    # This can also be used to refresh the system.
    Variable.all.each do |v|
      user = User.find(v.user_id)
      path = "/mixer/#{v.kind}/#{user.name}/#{v.name}"
      begin
        $OSCRUBY.send OSC::Message.new(path, v.value)
      rescue => ex
        puts "###########################################"
        puts "###########################################"
        puts "####> ERROR: OSC connection refused.."
        puts "###########################################"
        puts "###########################################"
      end
    end
  end

  def self.update_record(variable)
    if variable["kind"] == "mute" and variable["global"]
      self.update_global(variable)
    elsif variable["kind"] == "solo" or variable["kind"] == "record"
      self.update_global(variable)
    else
      record = Variable.find(variable["id"])
      record.value = variable["value"]
      record.save
      #record.update_attributes({ value: variable["value"] })
    end
  end

  def self.update_global(variable)
    Variable.where(kind: variable["kind"], name: variable["name"]).each do |v|
      v.update_attributes({ value: variable["value"] })
    end
  end

  private
    def broadcast_variable
      user = User.find(self.user_id)
      if self.kind != "vol"
        UserChannel.broadcast_to(user, { records: [self] })
      end

      if $OSCRUBY != nil
        if self.kind == "record"
          if self.value != 0
            preface = "record"
          else
            return 0
          end
        else
          preface = "mixer"
        end
        path = "/#{preface}/#{self.kind}/#{user.name}/#{self.name}"

        begin
          $OSCRUBY.send OSC::Message.new(path, self.value)
        rescue => ex
          puts "###########################################"
          puts "###########################################"
          puts "####> ERROR: OSC connection refused.."
          puts "###########################################"
          puts "###########################################"
        end

        #puts "----> OSCPATH: #{path}, VALUE: #{self.value}"
      else
        puts "##################################"
        puts "##################################"
        puts "####> ERROR: OSC doesnt work.."
        puts "##################################"
        puts "##################################"
      end

    end

end
