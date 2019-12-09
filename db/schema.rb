# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# This file is the source Rails uses to define your schema when running `rails
# db:schema:load`. When creating a new database, `rails db:schema:load` tends to
# be faster and is potentially less error prone than running all of your
# migrations from scratch. Old migrations may fail to apply correctly if those
# migrations use external dependencies or application code.
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 2019_12_09_043615) do

  # These are extensions that must be enabled in order to support this database
  enable_extension "plpgsql"

  create_table "channels", force: :cascade do |t|
    t.bigint "user_id"
    t.bigint "port_group_id"
    t.decimal "gain", default: "50.0"
    t.decimal "pan", default: "0.0"
    t.boolean "is_mute", default: false
    t.boolean "is_solo", default: false
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.index ["port_group_id"], name: "index_channels_on_port_group_id"
    t.index ["user_id"], name: "index_channels_on_user_id"
  end

  create_table "clips", force: :cascade do |t|
    t.string "name"
    t.string "path", default: ""
    t.decimal "gain", default: "0.0"
    t.bigint "start", default: 0
    t.bigint "end", default: 0
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.boolean "is_locked"
  end

  create_table "port_groups", force: :cascade do |t|
    t.string "name", null: false
    t.boolean "io", default: true
    t.boolean "is_global", default: false
    t.boolean "is_global_mute", default: false
    t.bigint "user_id"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.boolean "is_record", default: true
    t.index ["user_id"], name: "index_port_groups_on_user_id"
  end

  create_table "ports", force: :cascade do |t|
    t.string "name", null: false
    t.boolean "io", default: true
    t.string "path", default: ""
    t.decimal "pan", default: "0.0"
    t.bigint "port_group_id"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.index ["port_group_id"], name: "index_ports_on_port_group_id"
  end

  create_table "tag_clips", force: :cascade do |t|
    t.bigint "tag_id", null: false
    t.bigint "clip_id", null: false
    t.datetime "created_at", precision: 6, null: false
    t.datetime "updated_at", precision: 6, null: false
    t.index ["clip_id"], name: "index_tag_clips_on_clip_id"
    t.index ["tag_id"], name: "index_tag_clips_on_tag_id"
  end

  create_table "tags", force: :cascade do |t|
    t.string "name"
    t.datetime "created_at", precision: 6, null: false
    t.datetime "updated_at", precision: 6, null: false
  end

  create_table "users", force: :cascade do |t|
    t.string "email", default: "", null: false
    t.string "encrypted_password", default: "", null: false
    t.string "reset_password_token"
    t.datetime "reset_password_sent_at"
    t.datetime "remember_created_at"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.string "name"
    t.decimal "gain", default: "10.0"
    t.boolean "is_recording", default: false
    t.index ["email"], name: "index_users_on_email", unique: true
    t.index ["reset_password_token"], name: "index_users_on_reset_password_token", unique: true
  end

  create_table "wifis", force: :cascade do |t|
    t.string "name"
    t.string "network"
    t.string "password"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  add_foreign_key "channels", "port_groups"
  add_foreign_key "channels", "users"
  add_foreign_key "port_groups", "users"
  add_foreign_key "ports", "port_groups"
  add_foreign_key "tag_clips", "clips"
  add_foreign_key "tag_clips", "tags"
end
