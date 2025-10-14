# ARP query

from scapy.all import *
# from scapy.packet import Raw
# from scapy.layers import inet

ETH1 = 'enp1s0np0'
PCI1 = '0000:01:00.0'
IP1 = '10.120.101.111'
IPR = '10.120.101.11'

arp_request = ARP(pdst=IPR); arp_request
broadcast = Ether(dst="ff:ff:ff:ff:ff:ff"); broadcast

sendp(broadcast / arp_request, iface=ETH1)

answered_list = srp(broadcast / arp_request, timeout=1, verbose=False)[0]
answered_list

arp = {}; rev = {}
for i in answered_list:
    ip = i[1].psrc; mac = i[1].hwsrc
    arp[ip] = mac; rev[mac] = ip

arp

fake_frame = ARP(pdst=IP1); fake_frame
recv_direct = Ether(dst=arp[IPR]); recv_direct

sendp(recv_direct / fake_frame, iface=ETH1)
