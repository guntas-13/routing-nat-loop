from mininet.net import Mininet
from mininet.node import OVSSwitch, RemoteController, Controller, OVSKernelSwitch
from mininet.cli import CLI
from mininet.link import TCLink
from mininet.topo import Topo

class CustomTopo(Topo):
    
    def build(self):
        s1 = self.addSwitch('s1')
        s2 = self.addSwitch('s2')
        s3 = self.addSwitch('s3')
        s4 = self.addSwitch('s4')
        
        h1 = self.addHost('h1', ip='10.0.0.2/24')
        h2 = self.addHost('h2', ip='10.0.0.3/24')
        h3 = self.addHost('h3', ip='10.0.0.4/24')
        h4 = self.addHost('h4', ip='10.0.0.5/24')
        h5 = self.addHost('h5', ip='10.0.0.6/24')
        h6 = self.addHost('h6', ip='10.0.0.7/24')
        h7 = self.addHost('h7', ip='10.0.0.8/24')
        h8 = self.addHost('h8', ip='10.0.0.9/24')
        
        self.addLink(h1, s1, delay='5ms')
        self.addLink(h2, s1, delay='5ms')
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

topos = {'mytopo': (lambda: CustomTopo())}

if __name__ == '__main__':
    
    # net = Mininet(topo=CustomTopo(), controller=RemoteController, switch=OVSSwitch, link=TCLink)
    net = Mininet(topo=CustomTopo(), controller=Controller, switch=OVSKernelSwitch, link=TCLink)
    net.start()
    CLI(net)
    net.stop()