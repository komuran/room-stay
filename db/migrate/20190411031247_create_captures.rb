class CreateCaptures < ActiveRecord::Migration[5.2]
  def change
    create_table :captures do |t|
      t.string :pcdev
      t.string :mac
      t.integer :rssi
      t.integer :snr
      t.timestamps
    end
  end
end
