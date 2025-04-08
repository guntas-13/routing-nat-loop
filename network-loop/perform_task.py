import time, os
from mininet.net import Mininet
from mininet.node import OVSKernelSwitch, Controller
from mininet.cli import CLI
from mininet.link import TCLink
from mininet.topo import Topo

class CustomTopo(Topo): 
    def build(self):
        # Switches
        s1 = self.addSwitch('s1')
        s2 = self.addSwitch('s2')
        s3 = self.addSwitch('s3')
        s4 = self.addSwitch('s4')
        
        # Hosts with given IP addresses.
        h1 = self.addHost('h1', ip='10.0.0.2/24')
        h2 = self.addHost('h2', ip='10.0.0.3/24')
        h3 = self.addHost('h3', ip='10.0.0.4/24')
        h4 = self.addHost('h4', ip='10.0.0.5/24')
        h5 = self.addHost('h5', ip='10.0.0.6/24')
        h6 = self.addHost('h6', ip='10.0.0.7/24')
        h7 = self.addHost('h7', ip='10.0.0.8/24')
        h8 = self.addHost('h8', ip='10.0.0.9/24')
        
        # Host - Switch with 5ms delay
        self.addLink(h1, s1, delay='5ms')
        self.addLink(h2, s1, delay='5ms')
        self.addLink(h3, s2, delay='5ms')
        self.addLink(h4, s2, delay='5ms')
        self.addLink(h5, s3, delay='5ms')
        self.addLink(h6, s3, delay='5ms')
        self.addLink(h7, s4, delay='5ms')
        self.addLink(h8, s4, delay='5ms')
        
        # Switch - Switch with 7ms delay
        self.addLink(s1, s2, delay='7ms')
        self.addLink(s2, s3, delay='7ms')
        self.addLink(s3, s4, delay='7ms')
        self.addLink(s4, s1, delay='7ms')
        self.addLink(s1, s3, delay='7ms')

topos = {'mytopo': (lambda: CustomTopo())}

if __name__ == '__main__':
    net = Mininet(topo=CustomTopo(), controller=Controller, switch=OVSKernelSwitch, link=TCLink)
    net.start()  
    
    print("PingAll before STP...")
    net.pingAll()
    
    h1 = net.get('h1')
    h2 = net.git('h2')
    h3 = net.get('h3')
    h5 = net.get('h5')
    h7 = net.get('h7')
    h8 = net.get('h8')
    
    s1 = net.get('s1')
    s2 = net.get('s2')
    s3 = net.get('s3')
    s4 = net.get('s4')
    
    print("Starting STP on switches...")
    
    s1.cmd('ovs-vsctl set Bridge s1 stp_enable=true')
    s2.cmd('ovs-vsctl set Bridge s2 stp_enable=true')
    s3.cmd('ovs-vsctl set Bridge s3 stp_enable=true')
    s4.cmd('ovs-vsctl set Bridge s4 stp_enable=true')
    
    print("Waiting for STP to converge...")
    
    time.sleep(30)
    
    print("PingAll after STP...")
    net.pingAll()

    print("Starting pings...")
    
    os.makedir('./network-loop/PingResults/', ok_exists=True)
    for i in range(1, 4):
        h3.cmd(f'ping {h1.IP()} -w 30 > ./network-loop/PingResults/h3_h1_{i}.txt')
        h5.cmd(f'ping {h7.IP()} -w 30 > ./network-loop/PingResults/h5_h7_{i}.txt')
        h8.cmd(f'ping {h2.IP()} -w 30 > ./network-loop/PingResults/h8_h2_{i}.txt')
        
    CLI(net)
    net.stop()
