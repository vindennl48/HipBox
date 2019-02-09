PEOPLE = [ "james", "jesse", "mitch", "sean" ]

INPUTS = [
  { name: "james",    solo: true,  mute: {global: false, disabled: false} },
  { name: "jesse",    solo: true,  mute: {global: false, disabled: false} },
  { name: "mitch",    solo: true,  mute: {global: false, disabled: false} },
  { name: "sean",     solo: true,  mute: {global: false, disabled: false} },
  { name: "talkback", solo: false, mute: {global: true,  disabled: false} },
  { name: "click",    solo: false, mute: {global: false, disabled: false} },
  { name: "hp",       solo: false, mute: {global: false, disabled: true} },
]

PEOPLE.each do |person|

  user = User.create({
    name:     "#{person}",
    email:    "#{person}@lof.com",
    password: "password"
  })
  puts "person: #{person}, record: #{user.inspect}"

  INPUTS.each do |inp|

    variable = Variable.create({
      user_id: user.id,
      kind:    "vol",
      name:    inp[:name],
      value:   84,
    })
    puts "variable: id:#{variable.id}, name:#{variable.name}"

    if not inp[:mute][:disabled]

      variable = Variable.create({
        user_id: user.id,
        kind:    "mute",
        name:    inp[:name],
        global:  inp[:mute][:global],
        value:   0,
      })
      puts "variable: id:#{variable.id}, name:#{variable.name}"

    end
    if inp[:solo]

      variable = Variable.create({
        user_id: user.id,
        kind:    "solo",
        name:    inp[:name],
        value:   0,
      })
      puts "variable: id:#{variable.id}, name:#{variable.name}"

    end
  end

  # Record start button
  variable = Variable.create({
    user_id: user.id,
    kind:    "record",
    name:    "record",
    value:   0,
  })
  puts "variable: id:#{variable.id}, name:#{variable.name}"

end
