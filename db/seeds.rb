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
    User.create(name: user, email: "#{user}@hipbox.com", password: "password", password_confirmation: "password")
  end
end

puts "----> Creating Ports and PortGroups"

i = 1
users.each do |user|
  port_group = PortGroup.find_or_create_by(name: user, io: true, user: User.find_by(name: user))
  Port.find_or_create_by(name: user, io: true, path: "system:capture_#{i}", port_group: port_group)
  i += 1
end

puts "----> Done!"
