class Channel < ApplicationRecord
  belongs_to    :user
  belongs_to    :port_group

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
      "mixers"          => [],
      "channels"        => [],
      "in_ports"        => [],
      "in_port_groups"  => [],
      "out_ports"       => [],
      "out_port_groups" => [],
    }

    User.all.order("id").each do |user|
      out_port_group               = user.port_groups.find_by(io: false)
      mixer                        = user.as_json
      mixer["out_port_group_name"] = out_port_group.name

      aem["mixers"].push(mixer)
    end

    Channel.all.order("id").each do |ch|
      channel                    = ch.as_json
      channel["port_group_name"] = ch.port_group.name
      if ch.user
        channel["user_name"] = ch.user.name
      end

      aem["channels"].push(channel)
    end

    #In Ports
    Port.where(io: true).order("id").each do |port|
      in_port = port.as_json
      if port.port_group
        in_port["in_port_group_name"] = port.port_group.name
      end

      aem["in_ports"].push(in_port)
    end

    #In Port Groups
    PortGroup.where(io: true).order("id").each do |port_group|
      in_port_group = port_group.as_json
      if port_group.user
        in_port_group["user_name"] = port_group.user.name
      end

      aem["in_port_groups"].push(in_port_group)
    end

    #Out Port Groups
    PortGroup.where(io: false).order("id").each do |port_group|
      out_port_group = port_group.as_json
      if port_group.user
        out_port_group["user_name"] = port_group.user.name
      end
      out_port_group["port_left"]  = port_group.ports.find_by(pan: -1.0).as_json
      out_port_group["port_right"] = port_group.ports.find_by(pan:  1.0).as_json

      aem["out_port_groups"].push(out_port_group)
    end

    aem["samples"] = Sample.all.as_json
    aem["clicks"]  = Click.all.as_json

    $OSCRUBY.send OSC::Message.new("/rails", aem.to_json)
  end

  def self.activate_osc
    if $OSCRUBY == nil
      puts "OSCRUBY> Variable is not nil"
      puts "OSCRUBY> Starting OSC Client"

      Thread.new do
        loop do
          begin
            $OSCRUBY = OSC::Client.new 4000, '0.0.0.0'
            puts "OSCRUBY> OSC Client is activated"

            puts "OSCRUBY> Starting AEM process"
            Channel.start_aem

            Thread.exit
          rescue
            puts "OSCRUBY> Failed to initialize OSC Client.. Retry in 3 seconds.."
            sleep 3
          end
        end
      end
    end
  end

end
