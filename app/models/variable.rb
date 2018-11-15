class Variable < ApplicationRecord

  def self.process_incoming(name:nil, value:nil)
    result = false

    if name == "stop_all"
      result = true

      Variable.find_by(name:'play_toggle').update_attributes(status:false)
      Variable.find_by(name:'record_toggle').update_attributes(status:false)

      $OSCRUBY.send OSC::Message.new('/stop_playhead', 127)
      sleep(0.05)
      $OSCRUBY.send OSC::Message.new('/stop_loops', 1.0)
      sleep(0.05)
      $OSCRUBY.send OSC::Message.new('/advance_playhead', 1.0)

    elsif name == "record_toggle"
      $OSCRUBY.send OSC::Message.new('/SUPERAWESOMETIME', 1.0)
      result = true

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
        $OSCRUBY.send OSC::Message.new('/record_button_pretend', 1.0)

      end

    elsif name == "talkback_toggle"
      result = true
      talkback_toggle = Variable.find_by(name:'talkback_toggle')
      talkback_toggle.update_attributes(status:value)
      $OSCRUBY.send OSC::Message.new('/talkback_toggle', value)
    end

    return result

  end

end
