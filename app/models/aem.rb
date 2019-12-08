class AEM < Cmd::Debug
  # Do not start simple_client
  ## This is for debugging, SHOULD BE SET TO false
  @@NO_SIMPLE_CLIENT = false

  # Has the AEM been started yet
  @@IS_STARTED    = false
  # OSC Client for sending osc data
  @@OSC_CLIENT    = nil
  # If true, do not start or send OSC
  @@IS_OSC_CLOSED = false

  def self.deactivate
    @@IS_OSC_CLOSED = true
  end

  def self.activate
    @@IS_OSC_CLOSED = false
  end

  def self.status
    if @@OSC_CLIENT == nil or @@IS_OSC_CLOSED
      self.print "Status is false."
      self.print "    OSC_CLIENT:    #{@@OSC_CLIENT}"
      self.print "    IS_OSC_CLOSED: #{@@IS_OSC_CLOSED}"
      return false
    end
    return true
  end

  def self.start
    if @@IS_STARTED
      self.print "AEM is already started"
      return true
    else
      self.print "Starting AEM..."
    end

    if Cmd::Jack.start
      self.print "Starting AEM"
      if not @@NO_SIMPLE_CLIENT
        cmd = spawn("./aem/simple_client")
      end
      @@IS_STARTED = true
      self.print "AEM has been started"

      if self.establish_connection
        return true
      end
    end

    self.print "Launcing of AEM has been stopped.."
    return false
  end

  def self.establish_connection
    if @@OSC_CLIENT != nil or @@IS_OSC_CLOSED
      return false
    end

    Thread.new do
      loop do
        begin
          @@OSC_CLIENT = OSC::Client.new 4000, '0.0.0.0'
          self.print "OSC Client is activated"

          self.print "Starting AEM process"
          if self.reset
            self.print "AEM Started Successfully"
          else
            self.print "AEM did not start successfully.. Problem with OSC communication.."
          end

          Thread.exit
        rescue
          self.print "Failed to initialize OSC Client.. Retry in 3 seconds.."
          sleep 3
        end
      end
    end

    return true
  end

  def self.reset
    if not self.status
      return false
    end

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

    self.print "Sending initial json to AEM"
    if self.send("reset", aem)
      self.print "Initial json was sent successfully to AEM"
     return true
    else
      self.print "Failed to send initial json to AEM.."
      return false
    end
  end

  def self.send(method, message)
    if not self.status
      return false
    end

    begin
      @@OSC_CLIENT.send OSC::Message.new("/rails/#{method}", message.to_json)
      return true
    rescue
      self.print "Failed to send json to AEM..."
    end
    return false
  end

end
