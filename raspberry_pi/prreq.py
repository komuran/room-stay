from __future__ import print_function
from scapy.all import ( Dot11,
                        Dot11ProbeReq,
                        Dot11Elt,
                        RadioTap,
                        sendp,
                        hexdump)

SSID = '<SSID>' # <SSID>をアクセスポイントのSSIDに書き換える
iface = 'mon0'
sender = '<sender MAC address>' # <sender MAC address>に送信元のMACアドレスに書き換える

dot11 = Dot11(type=0, subtype=4, addr1='ff:ff:ff:ff:ff:ff', addr2=sender, addr3=sender)
pr = Dot11ProbeReq()
essid = Dot11Elt(ID='SSID',info=SSID, len=len(SSID))

frame = RadioTap()/dot11/pr/essid

sendp(frame, iface=iface, inter=0.001, count=10000)