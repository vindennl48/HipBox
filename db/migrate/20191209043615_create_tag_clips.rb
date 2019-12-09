class CreateTagClips < ActiveRecord::Migration[6.0]
  def change
    create_table :tag_clips do |t|
      t.references :tag, null: false, foreign_key: true
      t.references :clip, null: false, foreign_key: true

      t.timestamps
    end
  end
end
