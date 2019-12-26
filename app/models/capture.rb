class Capture < ApplicationRecord
    validates :pcdev, presence: true
    validates :mac, presence: true
    validates :rssi, presence: true
end
