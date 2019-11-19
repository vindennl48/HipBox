class Cmd::Jack < Cmd::Debug

  def self.start(
    device:      "USB",
    driver:      "asla",
    sample_rate: "44100",
    nperiods:    "2",
    buffer_size: "128"
  )
    self.print "Starting Jack Service"

    status = self.status

    if status and status != -1
      self.print "Jack is already running, stopping jack service.."
      self.stop
    end

    if status != -1
      self.device(device)
      self.driver(driver)
      self.sample_rate(sample_rate)
      self.nperiods(nperiods)
      self.buffer_size(buffer_size)

      cmd = `jack_control start`
      if not cmd[/Failed/]
        self.print "Jack service started successfully"
        return true
      else
        self.print "'jack_control start' failed to start.."
        return false
      end
    end

    self.print "Something went wrong..  Failed to start Jack"
    return false
  end

  def self.stop
    cmd = `jack_control stop`
    self.print "Jack service stopped successfully"
    return cmd
  end

  def self.device(a)
    cmd = `jack_control dps device hw:#{a}`
    self.print "Set device to #{a}"
    return cmd
  end

  def self.driver(a)
    cmd = `jack_control ds #{a}`
    self.print "Set driver to #{a}"
    return cmd
  end

  def self.sample_rate(a)
    cmd = `jack_control dps rate #{a}`
    self.print "Set sample rate to #{a}"
    return cmd
  end

  def self.nperiods(a)
    cmd = `jack_control dps nperiods #{a}`
    self.print "Set nperiods to #{a}"
    return cmd
  end

  def self.buffer_size(a)
    cmd = `jack_control dps period #{a}`
    self.print "Set buffer size to #{a}"
    return cmd
  end

  def self.status
    cmd = `jack_control status`
    if not cmd[/started/]
      self.print "Jack service is not running."
      return false
    elsif cmd[/started/]
      self.print "Jack service is running."
      return true
    end

    self.print "#######################################"
    self.print "#######################################"
    self.print "There is a problem with Jack service..."
    self.print "#######################################"
    self.print "#######################################"
    return -1
  end

end
