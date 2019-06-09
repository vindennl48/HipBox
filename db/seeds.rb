# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rails db:seed command (or created alongside the database with db:setup).
#
# Examples:
#
#   movies = Movie.create([{ name: 'Star Wars' }, { name: 'Lord of the Rings' }])
#   Character.create(name: 'Luke', movie: movies.first)


puts "----> Creating Users"

users = ["James","Jesse","Sean","Mitch"]

users.each do |user|
  if not User.exists? name: user
    User.create(name: user, email: "#{user}@hipbox.com", password: "password", password_confirmation: "password", gain: 10)
  end
end

puts "----> Creating Ports and PortGroups"

PortGroup.find_or_create_by(name: "James",    io: true,  user: User.find_by(name: "James"))
PortGroup.find_or_create_by(name: "Jesse",    io: true,  user: User.find_by(name: "Jesse"))
PortGroup.find_or_create_by(name: "Mitch",    io: true,  user: User.find_by(name: "Mitch"))
PortGroup.find_or_create_by(name: "Sean",     io: true,  user: User.find_by(name: "Sean"))
PortGroup.find_or_create_by(name: "Click",    io: true)
PortGroup.find_or_create_by(name: "Talkback", io: true,  is_global: true)

PortGroup.find_or_create_by(name: "James",    io: false, user: User.find_by(name: "James"))
PortGroup.find_or_create_by(name: "Jesse",    io: false, user: User.find_by(name: "Jesse"))
PortGroup.find_or_create_by(name: "Mitch",    io: false, user: User.find_by(name: "Mitch"))
PortGroup.find_or_create_by(name: "Sean",     io: false, user: User.find_by(name: "Sean"))

Port.find_or_create_by(name: "James",         io: true,  path: "system:capture_1",   port_group: PortGroup.find_by(name: "James",    io: true))
Port.find_or_create_by(name: "Jesse",         io: true,  path: "system:capture_2",   port_group: PortGroup.find_by(name: "Jesse",    io: true))
Port.find_or_create_by(name: "Talkback",      io: true,  path: "system:capture_3",   port_group: PortGroup.find_by(name: "Talkback", io: true))
Port.find_or_create_by(name: "Mitch",         io: true,  path: "system:capture_4",   port_group: PortGroup.find_by(name: "Mitch",    io: true))
Port.find_or_create_by(name: "Sean1",         io: true,  path: "system:capture_5",   port_group: PortGroup.find_by(name: "Sean",     io: true))
Port.find_or_create_by(name: "Sean2",         io: true,  path: "system:capture_6",   port_group: PortGroup.find_by(name: "Sean",     io: true))
Port.find_or_create_by(name: "-",             io: true,  path: "system:capture_7")
Port.find_or_create_by(name: "-",             io: true,  path: "system:capture_8")
Port.find_or_create_by(name: "Click",         io: true,  path: "hipbox:click",       port_group: PortGroup.find_by(name: "Click",    io: true))
Port.find_or_create_by(name: "Guit1",         io: true,  path: "hipbox:guit1",       port_group: PortGroup.find_by(name: "James",    io: true))
Port.find_or_create_by(name: "Guit2",         io: true,  path: "hipbox:guit2",       port_group: PortGroup.find_by(name: "Jesse",    io: true))
Port.find_or_create_by(name: "Bass",          io: true,  path: "hipbox:bass",        port_group: PortGroup.find_by(name: "Mitch",    io: true))
Port.find_or_create_by(name: "Drums",         io: true,  path: "hipbox:drums",       port_group: PortGroup.find_by(name: "Sean",     io: true))
Port.find_or_create_by(name: "Aux",           io: true,  path: "hipbox:aux")

Port.find_or_create_by(name: "-",             io: false, path: "system:playback_1")
Port.find_or_create_by(name: "-",             io: false, path: "system:playback_2")
Port.find_or_create_by(name: "Mitch_L",       io: false, path: "system:playback_3",  port_group: PortGroup.find_by(name: "Mitch",    io: false))
Port.find_or_create_by(name: "Mitch_R",       io: false, path: "system:playback_4",  port_group: PortGroup.find_by(name: "Mitch",    io: false))
Port.find_or_create_by(name: "James_L",       io: false, path: "system:playback_5",  port_group: PortGroup.find_by(name: "James",    io: false))
Port.find_or_create_by(name: "James_R",       io: false, path: "system:playback_6",  port_group: PortGroup.find_by(name: "James",    io: false))
Port.find_or_create_by(name: "Jesse_L",       io: false, path: "system:playback_7",  port_group: PortGroup.find_by(name: "Jesse",    io: false))
Port.find_or_create_by(name: "Jesse_R",       io: false, path: "system:playback_8",  port_group: PortGroup.find_by(name: "Jesse",    io: false))
Port.find_or_create_by(name: "Sean_L",        io: false, path: "system:playback_9",  port_group: PortGroup.find_by(name: "Sean",     io: false))
Port.find_or_create_by(name: "Sean_R",        io: false, path: "system:playback_10", port_group: PortGroup.find_by(name: "Sean",     io: false))

puts "----> Done!"
