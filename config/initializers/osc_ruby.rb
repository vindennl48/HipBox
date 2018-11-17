host  = (File.read("../HostIP/HostIP").scan /192\.168\.2\.[^(]*/i)[0]
local = '192.168.2.12'
addr  = Socket.ip_address_list
addr.each do |a|
  if a.ip_address.include? '192.168.2.'
    local = a.ip_address
    break
  end
end

puts "\n########################"
puts "Host:       #{host}"
puts "Ableton IP: #{local}:3002"
puts "Host IP:    #{local}:3000"
puts "########################\n\n"

$OSCRUBY = OSC::Client.new 3001, host

Thread.new do
  OSC.run do
    server = OSC::Server.new 3002, local

    server.add_pattern /.*/ do |*args|
      name  = args[0][1..-1]
      value = args[1]
      Variable.process_incoming(name: name, value: value)
      #record = Variable.find_by(name:name)
      #if record.type_of == 'boolean'
        #record.status = value == 1.0 ? true : false
      #else
        #record.value = (127.0 * value).to_i
      #end
      #record.save
    end

    ## from fcb1010 or other external midi device
    #server.add_pattern %r{variable/.*} do |*args|
      #vars = args[0][1..-1].split('/')
      #variable = vars[1]
      #ntype = vars[2]
      #value = args[1]
      #Note.broadcast_note(
        #Note.process_note(value, variable:variable, ntype:ntype))
    #end

    ## from ableton
    #server.add_pattern %r{midi/.*} do |*args|
      #vars = args[0][1..-1].split('/')
      #chan = vars[1].to_i
      #ntype = vars[2]
      #value = args[1].to_i
      #osc = args[0][5..-1]
      #if chan==15
        #Note.broadcast_note(
          #Note.process_note(value, osc:osc, ntype:ntype))
      #end
    #end

  end
end
