# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rails db:seed command (or created alongside the database with db:setup).
#
# Examples:
#
#   movies = Movie.create([{ name: 'Star Wars' }, { name: 'Lord of the Rings' }])
#   Character.create(name: 'Luke', movie: movies.first)

User.create([
  { email: 'mitch@lof.com', name: 'mitch', password: 'password' },
  { email: 'james@lof.com', name: 'james', password: 'password' },
  { email: 'jesse@lof.com', name: 'jesse', password: 'password' },
  { email: 'drums@lof.com', name: 'drums', password: 'password' },
])

Variable.create([
  { name: 'mitch_fx_locut',     status: false, type_of: 'boolean' },
  { name: 'mitch_fx_hicut',     status: false, type_of: 'boolean' },
  { name: 'mitch_fx_dist',      status: false, type_of: 'boolean' },
  { name: 'mitch_fx_delay',     status: false, type_of: 'boolean' },
  { name: 'mitch_fx_reverb',    status: false, type_of: 'boolean' },

  { name: 'talkback_toggle',    status: false, type_of: 'boolean' },
  { name: 'record_toggle',      status: false, type_of: 'boolean' },
  { name: 'hpr_toggle',         status: false, type_of: 'boolean' },

  { name: 'mitch_james_vol',    value:  84,    type_of: 'value' },
  { name: 'mitch_james_mute',   status: false, type_of: 'boolean' },
  { name: 'mitch_jesse_vol',    value:  84,    type_of: 'value' },
  { name: 'mitch_jesse_mute',   status: false, type_of: 'boolean' },
  { name: 'mitch_mitch_vol',    value:  84,    type_of: 'value' },
  { name: 'mitch_mitch_mute',   status: false, type_of: 'boolean' },
  { name: 'mitch_mitch_solo',   status: false, type_of: 'boolean' },
  { name: 'mitch_drums_vol',    value:  84,    type_of: 'value' },
  { name: 'mitch_drums_mute',   status: false, type_of: 'boolean' },
  { name: 'mitch_hp_vol',       value:  84,    type_of: 'value' },
  { name: 'mitch_click_vol',    value:  84,    type_of: 'value' },
  { name: 'mitch_talkback_vol', value:  84,    type_of: 'value' },

  { name: 'james_james_vol',    value:  84,    type_of: 'value' },
  { name: 'james_james_mute',   status: false, type_of: 'boolean' },
  { name: 'james_james_solo',   status: false, type_of: 'boolean' },
  { name: 'james_jesse_vol',    value:  84,    type_of: 'value' },
  { name: 'james_jesse_mute',   status: false, type_of: 'boolean' },
  { name: 'james_mitch_vol',    value:  84,    type_of: 'value' },
  { name: 'james_mitch_mute',   status: false, type_of: 'boolean' },
  { name: 'james_drums_vol',    value:  84,    type_of: 'value' },
  { name: 'james_drums_mute',   status: false, type_of: 'boolean' },
  { name: 'james_hp_vol',       value:  84,    type_of: 'value' },
  { name: 'james_click_vol',    value:  84,    type_of: 'value' },
  { name: 'james_talkback_vol', value:  84,    type_of: 'value' },

  { name: 'jesse_james_vol',    value:  84,    type_of: 'value' },
  { name: 'jesse_james_mute',   status: false, type_of: 'boolean' },
  { name: 'jesse_jesse_vol',    value:  84,    type_of: 'value' },
  { name: 'jesse_jesse_mute',   status: false, type_of: 'boolean' },
  { name: 'jesse_jesse_solo',   status: false, type_of: 'boolean' },
  { name: 'jesse_mitch_vol',    value:  84,    type_of: 'value' },
  { name: 'jesse_mitch_mute',   status: false, type_of: 'boolean' },
  { name: 'jesse_drums_vol',    value:  84,    type_of: 'value' },
  { name: 'jesse_drums_mute',   status: false, type_of: 'boolean' },
  { name: 'jesse_hp_vol',       value:  84,    type_of: 'value' },
  { name: 'jesse_click_vol',    value:  84,    type_of: 'value' },
  { name: 'jesse_talkback_vol', value:  84,    type_of: 'value' },

  { name: 'drums_james_vol',    value:  84,    type_of: 'value' },
  { name: 'drums_james_mute',   status: false, type_of: 'boolean' },
  { name: 'drums_jesse_vol',    value:  84,    type_of: 'value' },
  { name: 'drums_jesse_mute',   status: false, type_of: 'boolean' },
  { name: 'drums_mitch_vol',    value:  84,    type_of: 'value' },
  { name: 'drums_mitch_mute',   status: false, type_of: 'boolean' },
  { name: 'drums_drums_vol',    value:  84,    type_of: 'value' },
  { name: 'drums_drums_mute',   status: false, type_of: 'boolean' },
  { name: 'drums_drums_solo',   status: false, type_of: 'boolean' },
  { name: 'drums_hp_vol',       value:  84,    type_of: 'value' },
  { name: 'drums_click_vol',    value:  84,    type_of: 'value' },
  { name: 'drums_talkback_vol', value:  84,    type_of: 'value' },

])
