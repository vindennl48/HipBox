# global variable to send osc data to host
$OSCRUBY = OSC::Client.new 3001, '0.0.0.0'

# server to receive osc data from host
Thread.new do
  OSC.run do
    server = OSC::Server.new 3002, '0.0.0.0'

    # accept any osc patterns
    server.add_pattern /.*/ do |*args|
      name  = args[0][1..-1]
      value = args[1]
      Variable.process_incoming(name: name, value: value)
    end

  end
end
