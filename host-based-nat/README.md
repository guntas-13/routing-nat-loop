# Execution Guide

## Direct Run

To create the topology and run the tests you have to be in the `host-based-nat` directory. You can do this by running the following command:

```bash
cd host-based-nat
```

After that, you can run the following command to create the topology and run the tests:

```bash
sudo python3 perform_task.py
```

This will create the topology and run the tests. The output will be saved in the `IperfResults` directory and `PingResults` directory.

## Run Yourself

Go to the `host-based-nat` directory and run the following command:

```bash
sudo python3 nat.py 
```

This will create the topology and leave you in the mininet terminal. You can run the desired tests yourself to check the behavior of the topology.

**NOTE**: If you just want the topology then you can run the following command:

```bash
sudo python3 topo.py
```
