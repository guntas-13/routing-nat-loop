from mininet.net import Mininet
from mininet.node import Controller, OVSKernelSwitch
from mininet.link import TCLink
from mininet.topo import Topo
from mininet.cli import CLI
import time

class SimpleNATTopo(Topo):
    def build(self):
        # External switch
        s1 = self.addSwitch('s1', stp=True)

        # Hosts
        h1 = self.addHost('h1', ip='10.1.1.2/24')
        h2 = self.addHost('h2', ip='10.1.1.3/24')
        h3 = self.addHost('h3')  # NAT host — we will assign IPs manually
        h4 = self.addHost('h4', ip='172.16.0.2/24')

        # Internal links to NAT (h3)
        self.addLink(h1, h3, delay='5ms')
        self.addLink(h2, h3, delay='5ms')

        # External links (h3 to switch, h4 to switch)
        self.addLink(h3, s1, delay='5ms')  # h3 external side
        self.addLink(h4, s1, delay='5ms')

def run():
    net = Mininet(topo=SimpleNATTopo(), controller=Controller, switch=OVSKernelSwitch, link=TCLink)
    net.start()

    h1, h2, h3, h4 = net.get('h1', 'h2', 'h3', 'h4')

    # Create bridge br0 on h3 for internal interfaces
    h3.cmd('ip link add name br0 type bridge')
    h3.cmd('ip link set br0 up')
    h3.cmd('ip link set h3-eth0 master br0')  # h1-h3 link
    h3.cmd('ip link set h3-eth1 master br0')  # h2-h3 link

    # Assign internal IP to br0
    h3.cmd('ip addr add 10.1.1.1/24 dev br0')

    # Assign external IP to h3-eth2
    h3.setIP('172.16.0.1/24', intf='h3-eth2')  # h3 external side

    # Set default routes for internal hosts
    h1.cmd('ip route add default via 10.1.1.1')
    h2.cmd('ip route add default via 10.1.1.1')

    # Enable IP forwarding on h3
    h3.cmd('sysctl -w net.ipv4.ip_forward=1')

    # Setup NAT (MASQUERADE rule for internal subnet)
    h3.cmd('iptables -t nat -A POSTROUTING -s 10.1.1.0/24 -o h3-eth2 -j MASQUERADE')

    # Add route on h4 to reach the internal subnet 
    h4.cmd('ip route add default via 172.16.0.1 dev h4-eth0')

    print("Waiting 3 seconds for routes/NAT to settle...")
    time.sleep(3)

    CLI(net)
    net.stop()

if __name__ == '__main__':
    run()
