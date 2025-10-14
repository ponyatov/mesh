# ARP query

from scapy.all import *
from scapy.packet import Raw
from scapy.layers import inet

SENDIP = '10.120.101.111'
RECVIP = '10.120.101.11'

arp_request = ARP(pdst=RECVIP); arp_request
broadcast = Ether(dst="ff:ff:ff:ff:ff:ff"); broadcast

answered_list = srp(broadcast / arp_request, timeout=1, verbose=False)[0]
answered_list

for i in answered_list:
    print(i[1].psrc,i[1].hwsrc)
