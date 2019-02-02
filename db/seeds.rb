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


#User.create([
  #{ email: 'mitch@lof.com', name: 'mitch', password: 'password' },
  #{ email: 'james@lof.com', name: 'james', password: 'password' },
  #{ email: 'jesse@lof.com', name: 'jesse', password: 'password' },
  #{ email: 'drums@lof.com', name: 'drums', password: 'password' },
#])

#Variable.create([
  #{ name: 'stop_all',           status: false, type_of: 'boolean' },
  #{ name: 'play_button',        status: false, type_of: 'boolean' },
  #{ name: 'record_button',      status: false, type_of: 'boolean' },
  #{ name: 'advance_playhead',   status: false, type_of: 'boolean' },
  #{ name: 'stop_loops',         status: false, type_of: 'boolean' },
  #{ name: 'stop_playhead',      status: false, type_of: 'boolean' },



  #{ name: 'mitch_fx_locut',     status: false, type_of: 'boolean' },
  #{ name: 'mitch_fx_hicut',     status: false, type_of: 'boolean' },
  #{ name: 'mitch_fx_dist',      status: false, type_of: 'boolean' },
  #{ name: 'mitch_fx_delay',     status: false, type_of: 'boolean' },
  #{ name: 'mitch_fx_reverb',    status: false, type_of: 'boolean' },

  #{ name: 'talkback_toggle',    status: false, type_of: 'boolean' },
  #{ name: 'record_toggle',      status: false, type_of: 'boolean' },
  #{ name: 'play_toggle',        status: false, type_of: 'boolean' },
  #{ name: 'hpr_toggle',         status: false, type_of: 'boolean' },

  #{ name: 'mitch_james_vol',    value:  84,    type_of: 'value' },
  #{ name: 'mitch_james_mute',   status: false, type_of: 'boolean' },
  #{ name: 'mitch_jesse_vol',    value:  84,    type_of: 'value' },
  #{ name: 'mitch_jesse_mute',   status: false, type_of: 'boolean' },
  #{ name: 'mitch_mitch_vol',    value:  84,    type_of: 'value' },
  #{ name: 'mitch_mitch_mute',   status: false, type_of: 'boolean' },
  #{ name: 'mitch_mitch_solo',   status: false, type_of: 'boolean' },
  #{ name: 'mitch_drums_vol',    value:  84,    type_of: 'value' },
  #{ name: 'mitch_drums_mute',   status: false, type_of: 'boolean' },
  #{ name: 'mitch_hp_vol',       value:  84,    type_of: 'value' },
  #{ name: 'mitch_click_vol',    value:  84,    type_of: 'value' },
  #{ name: 'mitch_talkback_vol', value:  84,    type_of: 'value' },

  #{ name: 'james_james_vol',    value:  84,    type_of: 'value' },
  #{ name: 'james_james_mute',   status: false, type_of: 'boolean' },
  #{ name: 'james_james_solo',   status: false, type_of: 'boolean' },
  #{ name: 'james_jesse_vol',    value:  84,    type_of: 'value' },
  #{ name: 'james_jesse_mute',   status: false, type_of: 'boolean' },
  #{ name: 'james_mitch_vol',    value:  84,    type_of: 'value' },
  #{ name: 'james_mitch_mute',   status: false, type_of: 'boolean' },
  #{ name: 'james_drums_vol',    value:  84,    type_of: 'value' },
  #{ name: 'james_drums_mute',   status: false, type_of: 'boolean' },
  #{ name: 'james_hp_vol',       value:  84,    type_of: 'value' },
  #{ name: 'james_click_vol',    value:  84,    type_of: 'value' },
  #{ name: 'james_talkback_vol', value:  84,    type_of: 'value' },

  #{ name: 'jesse_james_vol',    value:  84,    type_of: 'value' },
  #{ name: 'jesse_james_mute',   status: false, type_of: 'boolean' },
  #{ name: 'jesse_jesse_vol',    value:  84,    type_of: 'value' },
  #{ name: 'jesse_jesse_mute',   status: false, type_of: 'boolean' },
  #{ name: 'jesse_jesse_solo',   status: false, type_of: 'boolean' },
  #{ name: 'jesse_mitch_vol',    value:  84,    type_of: 'value' },
  #{ name: 'jesse_mitch_mute',   status: false, type_of: 'boolean' },
  #{ name: 'jesse_drums_vol',    value:  84,    type_of: 'value' },
  #{ name: 'jesse_drums_mute',   status: false, type_of: 'boolean' },
  #{ name: 'jesse_hp_vol',       value:  84,    type_of: 'value' },
  #{ name: 'jesse_click_vol',    value:  84,    type_of: 'value' },
  #{ name: 'jesse_talkback_vol', value:  84,    type_of: 'value' },

  #{ name: 'drums_james_vol',    value:  84,    type_of: 'value' },
  #{ name: 'drums_james_mute',   status: false, type_of: 'boolean' },
  #{ name: 'drums_jesse_vol',    value:  84,    type_of: 'value' },
  #{ name: 'drums_jesse_mute',   status: false, type_of: 'boolean' },
  #{ name: 'drums_mitch_vol',    value:  84,    type_of: 'value' },
  #{ name: 'drums_mitch_mute',   status: false, type_of: 'boolean' },
  #{ name: 'drums_drums_vol',    value:  84,    type_of: 'value' },
  #{ name: 'drums_drums_mute',   status: false, type_of: 'boolean' },
  #{ name: 'drums_drums_solo',   status: false, type_of: 'boolean' },
  #{ name: 'drums_hp_vol',       value:  84,    type_of: 'value' },
  #{ name: 'drums_click_vol',    value:  84,    type_of: 'value' },
  #{ name: 'drums_talkback_vol', value:  84,    type_of: 'value' },

#])
