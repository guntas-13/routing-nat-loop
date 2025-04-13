# Execution Guide

## Direct Run

To create the topology and run the tests you have to be in the `network-loop` directory. You can do this by running the following command:

```bash
cd network-loop
```

After that, you can run the following command to create the topology and run the tests:

```bash
sudo python3 perform_task.py
```

This will create the topology and run the tests. The output will be saved in the `IperfResults` directory and `PingResults` directory.

## Run Yourself

If you just want the topology and run the tests yourself, you can go to the `network-loop` directory and run the following command:

```bash
sudo python3 topo.py
```

This will create the topology and leave you in the mininet terminal. You can run the desired tests yourself to check the behavior of the topology.

But remember that the topology consists of loops so you have to enable stp first which can be done by running the following command in another terminal:

```bash
sudo ovs-vsctl set Bridge <bridge_name> stp_enable=true
```

Mostly the <bridge_name> will be `s1`, `s2`, `s3` or `s4`. You can check the bridge names by running the following command:

```bash
sudo ovs-vsctl show
```

After enabling stp you have to wait for atleast 30 seconds for the stp to converge. After that you can run the tests yourself.
