# get ip for host
host  = (File.read("../HostIP/HostIP").scan /192\.168\.2\.[^(]*/i)[0]
local = '192.168.2.12'    # default ip

# get this server ip
addr  = Socket.ip_address_list
addr.each do |a|
  if a.ip_address.include? '192.168.2.'
    local = a.ip_address
    break
  end
end

puts "\n##############################"
puts "Ableton IP: #{local}:3002"
puts "------------------------------"
puts "Hipbox IP:  #{local}:3000"
puts "##############################\n\n"

# global variable to send osc data to host
$OSCRUBY = OSC::Client.new 3001, host

# server to receive osc data from host
Thread.new do
  OSC.run do
    server = OSC::Server.new 3002, local

    # accept any osc patterns
    server.add_pattern /.*/ do |*args|
      name  = args[0][1..-1]
      value = args[1]
      Variable.process_incoming(name: name, value: value)
    end

  end
end
