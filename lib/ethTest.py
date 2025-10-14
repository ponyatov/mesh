from scapy.all import *
from scapy.packet import Raw
from scapy.layers import inet

hex = 'E8EBD3934298112233445566'

rawbytes=bytes.fromhex(hex)

frame = inet.Ether(inet.raw(rawbytes));frame


FAKEMAC = '11:22:33:44:55:66'

ETH1 = 'enp1s0np0'
SRVIP = '10.120.100.39'

ETH1MAC = 'e8:eb:d3:93:42:98'
ETH2 = 'enp2s0np1'

CTL = 'enp6s18'
CTLMAC = 'bc:24:11:d4:a6:1f'
CTLIP = '10.120.100.39'

NOTEIP = '10.110.21.104'

TEST = 'enp1s0np1'
TESTIP = '10.120.101.11'
TESTMAC = 'e8:eb:d3:93:42:91'

SENDIP = '10.120.101.111'
RECVIP = '10.120.101.11'

sendp(frame, iface=ETH1)

BROADCAST = 'ff:ff:ff:ff:ff:ff'

f"tshark -i {CTL} -f 'arp or ip'"
f'ip neighbor show'

ether_layer = Ether(dst=BROADCAST,src=TESTMAC) ; ether_layer.show()

arp_request = ARP(pdst=SENDIP) ; arp_request.show()

arp_request_packet = ether_layer/arp_request ; arp_request_packet.show()

sendp(arp_request_packet, iface=TEST)
