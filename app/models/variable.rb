class Variable < ApplicationRecord
  after_save :broadcast_variable

  def self.process_incoming(record:nil, name:nil, value:nil)
    if record
      variable = Variable.find(record["id"])
      name = variable.name
      if record.key?("value")
        value = record["value"]
      else
        value = record["status"]
      end
    else
      variable = Variable.find_by(name:name)
      record = variable.attributes
      if record["type_of"] == 'boolean'
        record["status"] = value
      else
        record["value"] = value
      end
    end

    if name == "stop_all"
      self.stop_all

    elsif name == "record_toggle"
      self.record_toggle

    else
      variable.update_attributes(record)
      self.send_to_daw(variable)
    end

  end

  def self.stop_all
    Variable.find_by(name:'play_toggle').update_attributes(status:false)
    Variable.find_by(name:'record_toggle').update_attributes(status:false)

    $OSCRUBY.send OSC::Message.new('/stop_playhead', 127)
    sleep(0.05)
    $OSCRUBY.send OSC::Message.new('/stop_loops', 1.0)
    sleep(0.05)
    $OSCRUBY.send OSC::Message.new('/advance_playhead', 1.0)
  end

  def self.record_toggle
    record_toggle = Variable.find_by(name:'record_toggle')
    if not record_toggle.status

      record_toggle.update_attributes(status:true)

      play_toggle = Variable.find_by(name:'play_toggle')
      if not play_toggle.status
        play_toggle.update_attributes(status:true)
        $OSCRUBY.send OSC::Message.new('/advance_playhead', 1.0)
        sleep(0.05)
        $OSCRUBY.send OSC::Message.new('/play_button', 1.0)
      end

      sleep(1)
      # change to record_button in production
      $OSCRUBY.send OSC::Message.new('/record_button_pretend', 1.0)

    else # remove this in production
      self.stop_all
    end
  end

  private
    def broadcast_variable
      if self.type_of == 'boolean'
        OscDataChannel.broadcast_to(self, { record: self })
      end
    end

    def self.send_to_daw(variable)
      $OSCRUBY.send OSC::Message.new(self.get_osc(variable), self.parse_data(variable))
    end

    def self.get_osc(variable)
      return "/#{variable.name}"
    end

    def self.parse_data(variable)
      if variable.type_of == 'boolean'
        return (variable.status ? 1.0 : 0.0)
      else #elsif variable.type_of == 'value'
        return (variable.value.to_f / 127.0).round(3)
      end
    end

end
