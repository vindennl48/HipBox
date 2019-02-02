# global variable to send osc data to host
$OSCRUBY = OSC::Client.new 3001, '0.0.0.0'

# server to receive osc data from host
Thread.new do
  OSC.run do
    server = OSC::Server.new 3002, '0.0.0.0'

    # accept any osc patterns
    server.add_pattern /.*/ do |*args|
      path  = args[0]
      value = args[1]
      Variable.process_osc(path, value)
    end

  end
end
