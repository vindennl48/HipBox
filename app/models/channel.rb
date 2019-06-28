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

  def self.activate_osc
    #if $OSCRUBY == nil
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
    #end
  end

end
