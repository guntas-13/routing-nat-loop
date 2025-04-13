from mininet.net import Mininet
from mininet.node import OVSKernelSwitch, Controller
from mininet.cli import CLI
from mininet.link import TCLink
from mininet.topo import Topo
from time import sleep
import os

class CustomTopo(Topo):
    def build(self):
        s1 = self.addSwitch('s1')
        s2 = self.addSwitch('s2')
        s3 = self.addSwitch('s3')
        s4 = self.addSwitch('s4')

        h1 = self.addHost('h1', ip='10.1.1.2/24')
        h2 = self.addHost('h2', ip='10.1.1.3/24')
        h3 = self.addHost('h3', ip='10.0.0.4/24')
        h4 = self.addHost('h4', ip='10.0.0.5/24')
        h5 = self.addHost('h5', ip='10.0.0.6/24')
        h6 = self.addHost('h6', ip='10.0.0.7/24')
        h7 = self.addHost('h7', ip='10.0.0.8/24')
        h8 = self.addHost('h8', ip='10.0.0.9/24')
        h9 = self.addHost('h9', ip='172.16.10.10/24') # NAT

        self.addLink(h9, s1, delay='5ms')
        self.addLink(h1, h9, delay='5ms')
        self.addLink(h2, h9, delay='5ms')
        self.addLink(h3, s2, delay='5ms')
        self.addLink(h4, s2, delay='5ms')
        self.addLink(h5, s3, delay='5ms')
        self.addLink(h6, s3, delay='5ms')
        self.addLink(h7, s4, delay='5ms')
        self.addLink(h8, s4, delay='5ms')

        self.addLink(s1, s2, delay='7ms')
        self.addLink(s2, s3, delay='7ms')
        self.addLink(s3, s4, delay='7ms')
        self.addLink(s4, s1, delay='7ms')
        self.addLink(s1, s3, delay='7ms')

def configure_and_test(net):
    h1, h2, h9, h3, h4, h5, h6, h7, h8 = net.get('h1', 'h2', 'h9', 'h3', 'h4', 'h5', 'h6', 'h7', 'h8')

    print("PingAll before NAT Configuration...")
    net.pingAll()
    # Create a bridge on h9 for internal traffic from h1/h2
    
    print("Configuring NAT on h9...")
    
    h9.cmd('ip link add br0 type bridge')
    h9.cmd('ip link set br0 up')
    h9.cmd('ip link set h9-eth1 master br0')  # h1-h9
    h9.cmd('ip link set h9-eth2 master br0')  # h2-h9

    h9.cmd('ip addr add 10.1.1.1/24 dev br0') # IP for internal side

    # Assign external IP to h9 (already has via h9-eth0)
    h9.setIP('172.16.10.10/24', intf='h9-eth0')
    h9.cmd('ip addr add 10.0.0.100/24 dev h9-eth0')

    # Set default route on h1 and h2 to internal IP of h9
    h1.cmd('ip route add default via 10.1.1.1')
    h2.cmd('ip route add default via 10.1.1.1')

    # Enable IP forwarding
    h9.cmd('sysctl -w net.ipv4.ip_forward=1')

    # NAT rule to allow h1/h2 to talk to outside world
    h9.cmd('iptables -t nat -A POSTROUTING -s 10.1.1.0/24 -o h9-eth0 -j MASQUERADE')
    
    # External hosts: route to h9's external IP (172.16.10.10)
    for host in [h3, h4, h5, h6, h7, h8]:
        host.cmd('ip route add 172.16.10.10 via 10.0.0.100') # Route to NAT host
        host.cmd('ip route add 10.1.1.0/24 via 10.0.0.100') # Route to internal network
    
    print("PingAll after NAT Configuration but before enabling STP...")
    net.pingAll()
    
    print("Starting STP on switches...")
    for switch in ['s1', 's2', 's3', 's4']:
        net.get(switch).cmd('ovs-vsctl set bridge {} stp_enable=true'.format(switch))
    print("Waiting for STP to converge...")
    sleep(30)

    print("PingAll after STP...")
    net.pingAll()

    print("Starting pings...")
    
    os.makedir('./host-based-nat/PingResults/', ok_exists=True)
    os.makedir('./host-based-nat/IperfResults/', ok_exists=True)
    
    for i in range(1, 4):
        h1.cmd(f'ping {h5.IP()} -w 30 > ./network-loop/PingResults/h1_h5_{i}.txt')
        h2.cmd(f'ping {h3.IP()} -w 30 > ./network-loop/PingResults/h2_h3_{i}.txt')
        h8.cmd(f'ping {h1.IP()} -w 30 > ./network-loop/PingResults/h8_h1_{i}.txt')
        h6.cmd(f'ping {h2.IP()} -w 30 > ./network-loop/PingResults/h6_h2_{i}.txt')
        
    print("Starting iperf server on h1 and h8...")
    h1.cmd('iperf3 -s -p 1212 -D')
    h8.cmd('iperf3 -s -p 1212 -D')
    
    print("Starting iperf client on h2 and h6...")
    for i in range(1, 4):
        h2.cmd(f'iperf3 -c {h8.IP()} -p 1212 -t 120 > ./host-based-nat/IperfResults/h2_iperf_h8_{i}.txt')
        sleep(2)
        h6.cmd(f'iperf3 -c {h1.IP()} -p 1212 -t 120 > ./host-based-nat/IperfResults/h6_iperf_h1_{i}.txt')
        sleep(2) 
    
    print("Ping and iperf tests completed.")
    
    
if __name__ == '__main__':
    net = Mininet(topo=CustomTopo(), controller=Controller, switch=OVSKernelSwitch, link=TCLink)
    net.start()
    configure_and_test(net)
    CLI(net)
    net.stop()
