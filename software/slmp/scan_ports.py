from scapy.all import *
from scapy.layers.inet import TCP, IP, UDP, ICMP

def print_ports(port, state):
        print("%s | %s" % (port, state))


def syn_scan(target, ports):
        print("syn scan on, %s with ports %s" % (target, ports))
        sport = RandShort()
        for port in ports:
                pkt = sr1(IP(dst=target)/TCP(sport=sport, dport=port, flags="S"), timeout=1, verbose=0)
                if pkt != None:
                        if pkt.haslayer(TCP):
                                if pkt[TCP].flags == 20:
                                        print_ports(port, "Closed")
                                elif pkt[TCP].flags == 18:
                                        print_ports(port, "Open")
                                else:
                                        print_ports(port, "TCP packet resp / filtered")
                        elif pkt.haslayer(ICMP):
                                print_ports(port, "ICMP resp / filtered")
                        else:
                                print_ports(port, "Unknown resp")
                                print(pkt.summary())
                else:
                        print_ports(port, "Unanswered")


def udp_scan(target, ports):
        print("udp scan on, %s with ports %s" % (target, ports))
        for port in ports:
                pkt = sr1(IP(dst=target)/UDP(sport=port, dport=port), timeout=2, verbose=0)
                if pkt == None:
                        print_ports(port, "Open / filtered")
                else:
                        if pkt.haslayer(ICMP):
                                print_ports(port, "Closed")
                        elif pkt.haslayer(UDP):
                                print_ports(port, "Open / filtered")
                        else:
                                print_ports(port, "Unknown")
                                print(pkt.summary())


syn_scan('192.168.88.2', [i for i in range(4900, 5000)])
udp_scan('192.168.88.2', [i for i in range(4900, 5000)])
