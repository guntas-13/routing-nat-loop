from mininet.net import Mininet
from mininet.node import OVSKernelSwitch, Controller, Host
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

        # Hosts with given IP addresses
        h1 = self.addHost('h1', ip='10.1.1.2/24')
        h2 = self.addHost('h2', ip='10.1.1.3/24')
        h3 = self.addHost('h3', ip='10.0.0.4/24')
        h4 = self.addHost('h4', ip='10.0.0.5/24')
        h5 = self.addHost('h5', ip='10.0.0.6/24')
        h6 = self.addHost('h6', ip='10.0.0.7/24')
        h7 = self.addHost('h7', ip='10.0.0.8/24')
        h8 = self.addHost('h8', ip='10.0.0.9/24')
        h9 = self.addHost('h9', ip='172.16.10.10/24')

        # Host - Switch with 5ms delay
        self.addLink(h9, s1, delay='5ms')
        self.addLink(h1, h9, delay='5ms')
        self.addLink(h2, h9, delay='5ms')
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

def configure_nat_rules(net):
    h9 = net.get('h9')
    h1 = net.get('h1')
    h2 = net.get('h2')
    
    # Remove any existing IPs on the internal interfaces
    h9.cmd('ip addr flush dev h9-eth1')
    h9.cmd('ip addr flush dev h9-eth2')
    
    # Create a new bridge "br0" using iproute2 commands
    h9.cmd('ip link add name br0 type bridge')
    h9.cmd('ip link set dev br0 up')
    
    # Add h9-eth1 and h9-eth2 to the bridge
    h9.cmd('ip link set dev h9-eth1 master br0')
    h9.cmd('ip link set dev h9-eth2 master br0')
    
    # Assign IP to the bridge - internal gateway
    h9.cmd('ip addr add 10.1.1.1/24 dev br0')
    
    # External interface remains with its public IP (already set)
    h9.setIP('172.16.10.10/24', intf='h9-eth0')
    
    # Enable IP forwarding on H9
    h9.cmd('sysctl -w net.ipv4.ip_forward=1')
    
    # Configure NAT rules (masquerade on the external interface)
    h9.cmd('iptables -t nat -A POSTROUTING -o h9-eth0 -j MASQUERADE')
    h9.cmd('iptables -A FORWARD -i h9-eth0 -o br0 -m state --state RELATED,ESTABLISHED -j ACCEPT')
    h9.cmd('iptables -A FORWARD -i br0 -o h9-eth0 -j ACCEPT')
    
    # Set default routes for H1 and H2 to route through H9's internal IP (the bridge IP)
    h1.cmd('ip route add default via 10.1.1.1')
    h2.cmd('ip route add default via 10.1.1.1')

if __name__ == '__main__':
    net = Mininet(topo=CustomTopo(), controller=Controller, switch=OVSKernelSwitch, link=TCLink)
    net.start()
    configure_nat_rules(net)
    CLI(net)
    net.stop()
