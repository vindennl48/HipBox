$OSCRUBY = OSC::Client.new 3001, '192.168.2.7'

#Thread.new do
  #OSC.run do
    #server = OSC::Server.new 4005, '0.0.0.0'

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

  #end
#end
