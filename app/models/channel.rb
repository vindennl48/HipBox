class Channel < ApplicationRecord
  belongs_to    :user
  belongs_to    :port_group
  after_update  :send_to_aem

  def self.generate_channels(user)
    PortGroup.where(io: true).order("id").each do |port_group|
      Channel.find_or_create_by(
        user:       user,
        port_group: port_group,
      )
    end
  end

  def self.start_aem
    aem = {
      "mixers"  => [],
      "samples" => [],
      "clicks"  => [],
    }

    User.all.order("id").each do |user|
      mixer = user.as_json(include: {
        channels: {include: {port_group: {include: :ports}}},
      })

      mixer["out_port_group"] = user.port_groups.find_by(io: false).as_json(include: :ports)
      mixer["in_port_groups"] = user.port_groups.where(io: true).as_json(include: :ports)

      aem["mixers"].push(mixer)
    end

    aem["samples"] = Sample.all.as_json
    aem["clicks"]  = Click.all.as_json

    $OSCRUBY.send OSC::Message.new("/rails", aem.to_json)
  end

  def self._osc_ruby_service_start
    loop do
      begin
        $OSCRUBY = OSC::Client.new 4000, '0.0.0.0'
        puts "\nOSCRUBY> OSC Client is activated\n\n"

        puts "\nOSCRUBY> Starting AEM process\n\n"
        Channel.start_aem
        puts "\nOSCRUBY> AEM Started Successfully\n\n"

        #Thread.exit
        return true
      rescue
        puts "\nOSCRUBY> Failed to initialize OSC Client.. Retry in 3 seconds..\n\n"
        sleep 3
      end
    end
  end

  def self.activate_osc
    if $OSCRUBY == nil
      puts "\nOSCRUBY> Starting OSC Client\n\n"

      Thread.new do
        # Check Jack to see if it is running yet
        cmd = `jack_control status`
        if not cmd[/started/]
          puts "\nOSCRUBY> Starting Jack Service\n\n"

          # Start jack service
          cmd = `jack_control dps device hw:USB` # set device
          cmd = `jack_control ds  asla`          # set driver
          cmd = `jack_control dps rate 44100`    # set sample rate
          cmd = `jack_control dps nperiods 2`    # set nperiods
          cmd = `jack_control dps period 128`    # set buffer size
          cmd = `jack_control start`             # start jack
          if not cmd[/Failed/]
            puts "\nOSCRUBY> Jack Service Started Successfully. Starting AEM\n\n"
            if Channel._osc_ruby_service_start
              Thread.exit
            end
          else
            puts "\nOSCRUBY> ERROR> 'jack_control start' failed to start..\n\n"
          end
        elsif cmd[/started/]
          puts "\nOSCRUBY> Jack Service Already Started. Starting AEM\n\n"
          if Channel._osc_ruby_service_start
            Thread.exit
          end
        else
          puts ""
          puts "OSCRUBY> ERROR> Current 'jack_control status' is unrecognizable.."
          puts "                Could not start AEM.."
          puts ""
        end
      end

    else
      puts "\nOSCRUBY> OSC Client is Already Started.\n\n"
    end
  end

  private

  def send_to_aem
    aem = { "channel" => self }
    $OSCRUBY.send OSC::Message.new("/rails", aem.to_json)
  end

end
