class CreateCaptures < ActiveRecord::Migration[5.2]
  def change
    create_table :captures do |t|
      t.string :capture

      t.timestamps
    end
  end
end
