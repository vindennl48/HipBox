class AEM
  # OSC Client for sending osc data
  @@CLIENT    = nil
  # If true, do not start or send OSC
  @@IS_DEACTIVATED = false

  def self.deactivate
    @@IS_DEACTIVATED = true
  end

  def self.activate
    @@IS_DEACTIVATED = false
  end

  def self.is_deactivated
    return @@IS_DEACTIVATED
  end

  def self.establish_connection
    if @@CLIENT != nil or @@IS_DEACTIVATED
      return 0
    end

    Thread.new do
      loop do
        begin
          @@CLIENT = OSC::Client.new 4000, '0.0.0.0'
          puts "AEM-MODEL> OSC Client is activated"

          puts "AEM-MODEL> Starting AEM process"
          if AEM.init
            puts "AEM-MODEL> AEM Started Successfully"
          else
            puts "AEM-MODEL> AEM did not start successfully.. Problem with OSC communication.."
          end

          Thread.exit
        rescue
          puts "AEM-MODEL> Failed to initialize OSC Client.. Retry in 3 seconds.."
          sleep 3
        end
      end
    end
  end

  def self.init
    # also need to send selected song clips, click samples, etc.
    aem = { "mixers" => [] }

    User.all.order("id").each do |user|
      mixer = user.as_json(include: {
        channels: {include: {port_group: {include: :ports}}},
      })

      mixer["out_port_group"] = user.port_groups.find_by(io: false).as_json(include: :ports)
      mixer["in_port_groups"] = user.port_groups.where(io: true).as_json(include: :ports)

      aem["mixers"].push(mixer)
    end

    puts "AEM-MODEL> Sending initial json to AEM"
    if AEM.send("init", aem)
      puts "AEM-MODEL> Initial json was sent successfully to AEM"
      return 1
    else
      puts "AEM-MODEL> Failed to send initial json to AEM.."
      return 0
    end
  end

  def self.send(method, message)
    if @@CLIENT == nil or @@IS_DEACTIVATED
      return 0
    end

    begin
      @@CLIENT.send OSC::Message.new("/rails/#{method}", message.to_json)
      return 1
    rescue
      puts "AEM-MODEL> Failed to send json to AEM..."
    end
    return 0
  end

end
