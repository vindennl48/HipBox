# global variable to send osc data to host
$OSCRUBY = OSC::Client.new 4000, '0.0.0.0'

Thread.new do
  loop do
    begin
      puts "----> SENDING MESSAGE TO OSC"
      $OSCRUBY.send OSC::Message.new("/rails", '{"happy":true, "pi": 3.14159, "inPorts": [{"name": "Mitch"}, {"name": "Jesse"}]}')
    rescue
      $OSCRUBY = OSC::Client.new 4000, '0.0.0.0'
    end
    sleep 3
  end
end
