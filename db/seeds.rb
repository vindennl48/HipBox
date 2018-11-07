# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rails db:seed command (or created alongside the database with db:setup).
#
# Examples:
#
#   movies = Movie.create([{ name: 'Star Wars' }, { name: 'Lord of the Rings' }])
#   Character.create(name: 'Luke', movie: movies.first)

User.create([
  { email:'mitch@lof.com', name:'mitch', password:'password' },
  { email:'james@lof.com', name:'james', password:'password' },
  { email:'jesse@lof.com', name:'jesse', password:'password' },
  { email:'drums@lof.com', name:'drums', password:'password' },
])

Variable.create([
  { name:'talkback_toggle', status:false },
  { name:'record_toggle', status:false },
  { name:'hpr_toggle', status:false },

  { name:'mitch_james_vol', value:84 },
  { name:'mitch_james_mute', status:false },
  { name:'mitch_jesse_vol', value:84 },
  { name:'mitch_jesse_mute', status:false },
  { name:'mitch_mitch_vol', value:84 },
  { name:'mitch_mitch_mute', status:false },
  { name:'mitch_mitch_solo', status:false },
  { name:'mitch_drums_vol', value:84 },
  { name:'mitch_drums_mute', status:false },
  { name:'mitch_hp_vol', value:84 },
  { name:'mitch_click_vol', value:84 },
  { name:'mitch_talkback_vol', value:84 },

  { name:'james_james_vol', value:84 },
  { name:'james_james_mute', status:false },
  { name:'james_james_solo', status:false },
  { name:'james_jesse_vol', value:84 },
  { name:'james_jesse_mute', status:false },
  { name:'james_mitch_vol', value:84 },
  { name:'james_mitch_mute', status:false },
  { name:'james_drums_vol', value:84 },
  { name:'james_drums_mute', status:false },
  { name:'james_hp_vol', value:84 },
  { name:'james_click_vol', value:84 },
  { name:'james_talkback_vol', value:84 },

  { name:'jesse_james_vol', value:84 },
  { name:'jesse_james_mute', status:false },
  { name:'jesse_jesse_vol', value:84 },
  { name:'jesse_jesse_mute', status:false },
  { name:'jesse_jesse_solo', status:false },
  { name:'jesse_mitch_vol', value:84 },
  { name:'jesse_mitch_mute', status:false },
  { name:'jesse_drums_vol', value:84 },
  { name:'jesse_drums_mute', status:false },
  { name:'jesse_hp_vol', value:84 },
  { name:'jesse_click_vol', value:84 },
  { name:'jesse_talkback_vol', value:84 },

  { name:'drums_james_vol', value:84 },
  { name:'drums_james_mute', status:false },
  { name:'drums_jesse_vol', value:84 },
  { name:'drums_jesse_mute', status:false },
  { name:'drums_mitch_vol', value:84 },
  { name:'drums_mitch_mute', status:false },
  { name:'drums_drums_vol', value:84 },
  { name:'drums_drums_mute', status:false },
  { name:'drums_drums_solo', status:false },
  { name:'drums_hp_vol', value:84 },
  { name:'drums_click_vol', value:84 },
  { name:'drums_talkback_vol', value:84 },

])
