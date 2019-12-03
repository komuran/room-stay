class Capture < ApplicationRecord
    validates :capture, presence: true
    validates :rssi, presence: true
    validates :snr, presence: true
end
