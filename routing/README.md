# Routing Algorithm: Distributed Asynchronous Distance Vector Routing

```bash
gcc distance_vector.c node0.c node1.c node2.c node3.c -o route
```

```bash
./route
```

Enter the `TRACE=2`

```bash
Enter TRACE:2
                via
   D0 |    1     2    3
  ----|-----------------
     1|    1   999   999
dest 2|  999     3   999
     3|  999   999     7
             via
   D1 |    0     2
  ----|-----------
     0|    1   999
dest 2|  999     1
     3|  999   999
                via
   D2 |    0     1    3
  ----|-----------------
     0|    3   999   999
dest 1|  999     1   999
     3|  999   999     2
             via
   D3 |    0     2
  ----|-----------
     0|    7   999
dest 1|  999   999
     2|  999     2
MAIN: rcv event, t=0.420, at 3 src: 0, dest: 3, contents:   0   1   3   7
             via
   D3 |    0     2
  ----|-----------
     0|    7   999
dest 1|    8   999
     2|   10     2
MAIN: rcv event, t=0.689, at 3 src: 2, dest: 3, contents:   3   1   0   2
             via
   D3 |    0     2
  ----|-----------
     0|    7     5
dest 1|    8     3
     2|   10     2
MAIN: rcv event, t=0.697, at 1 src: 0, dest: 1, contents:   0   1   3   7
             via
   D1 |    0     2
  ----|-----------
     0|    1   999
dest 2|    4     1
     3|    8   999
MAIN: rcv event, t=0.980, at 0 src: 1, dest: 0, contents:   1   0   1 999
                via
   D0 |    1     2    3
  ----|-----------------
     1|    1   999   999
dest 2|    2     3   999
     3|  999   999     7
MAIN: rcv event, t=1.270, at 2 src: 0, dest: 2, contents:   0   1   3   7
                via
   D2 |    0     1    3
  ----|-----------------
     0|    3   999   999
dest 1|    4     1   999
     3|   10   999     2
MAIN: rcv event, t=1.532, at 1 src: 2, dest: 1, contents:   3   1   0   2
             via
   D1 |    0     2
  ----|-----------
     0|    1     4
dest 2|    4     1
     3|    8     3
MAIN: rcv event, t=2.007, at 2 src: 1, dest: 2, contents:   1   0   1 999
                via
   D2 |    0     1    3
  ----|-----------------
     0|    3     2   999
dest 1|    4     1   999
     3|   10   999     2
MAIN: rcv event, t=2.147, at 0 src: 2, dest: 0, contents:   3   1   0   2
                via
   D0 |    1     2    3
  ----|-----------------
     1|    1     4   999
dest 2|    2     3   999
     3|  999     5     7
MAIN: rcv event, t=2.336, at 1 src: 0, dest: 1, contents:   0   1   2   7
             via
   D1 |    0     2
  ----|-----------
     0|    1     4
dest 2|    3     1
     3|    8     3
MAIN: rcv event, t=2.786, at 3 src: 0, dest: 3, contents:   0   1   2   7
             via
   D3 |    0     2
  ----|-----------
     0|    7     5
dest 1|    8     3
     2|    9     2
MAIN: rcv event, t=3.045, at 0 src: 3, dest: 0, contents:   7 999   2   0
                via
   D0 |    1     2    3
  ----|-----------------
     1|    1     4   999
dest 2|    2     3     9
     3|  999     5     7
MAIN: rcv event, t=3.485, at 1 src: 2, dest: 1, contents:   2   1   0   2
             via
   D1 |    0     2
  ----|-----------
     0|    1     3
dest 2|    3     1
     3|    8     3
MAIN: rcv event, t=3.488, at 1 src: 0, dest: 1, contents:   0   1   2   5
             via
   D1 |    0     2
  ----|-----------
     0|    1     3
dest 2|    3     1
     3|    6     3
MAIN: rcv event, t=3.613, at 2 src: 3, dest: 2, contents:   7 999   2   0
                via
   D2 |    0     1    3
  ----|-----------------
     0|    3     2     9
dest 1|    4     1   999
     3|   10   999     2
MAIN: rcv event, t=4.725, at 3 src: 2, dest: 3, contents:   2   1   0   2
             via
   D3 |    0     2
  ----|-----------
     0|    7     4
dest 1|    8     3
     2|    9     2
MAIN: rcv event, t=4.940, at 0 src: 3, dest: 0, contents:   7   8   2   0
                via
   D0 |    1     2    3
  ----|-----------------
     1|    1     4    15
dest 2|    2     3     9
     3|  999     5     7
MAIN: rcv event, t=5.234, at 2 src: 3, dest: 2, contents:   7   8   2   0
                via
   D2 |    0     1    3
  ----|-----------------
     0|    3     2     9
dest 1|    4     1    10
     3|   10   999     2
MAIN: rcv event, t=5.264, at 0 src: 3, dest: 0, contents:   5   3   2   0
                via
   D0 |    1     2    3
  ----|-----------------
     1|    1     4    10
dest 2|    2     3     9
     3|  999     5     7
MAIN: rcv event, t=6.173, at 3 src: 0, dest: 3, contents:   0   1   2   5
             via
   D3 |    0     2
  ----|-----------
     0|    7     4
dest 1|    8     3
     2|    9     2
MAIN: rcv event, t=6.887, at 0 src: 1, dest: 0, contents:   1   0   1   8
                via
   D0 |    1     2    3
  ----|-----------------
     1|    1     4    10
dest 2|    2     3     9
     3|    9     5     7
MAIN: rcv event, t=6.921, at 0 src: 1, dest: 0, contents:   1   0   1   3
                via
   D0 |    1     2    3
  ----|-----------------
     1|    1     4    10
dest 2|    2     3     9
     3|    4     5     7
MAIN: rcv event, t=6.969, at 3 src: 0, dest: 3, contents:   0   1   2   4
             via
   D3 |    0     2
  ----|-----------
     0|    7     4
dest 1|    8     3
     2|    9     2
MAIN: rcv event, t=7.168, at 2 src: 3, dest: 2, contents:   5   3   2   0
                via
   D2 |    0     1    3
  ----|-----------------
     0|    3     2     7
dest 1|    4     1     5
     3|   10   999     2
MAIN: rcv event, t=8.227, at 1 src: 0, dest: 1, contents:   0   1   2   4
             via
   D1 |    0     2
  ----|-----------
     0|    1     3
dest 2|    3     1
     3|    5     3
MAIN: rcv event, t=8.244, at 0 src: 2, dest: 0, contents:   2   1   0   2
                via
   D0 |    1     2    3
  ----|-----------------
     1|    1     4    10
dest 2|    2     3     9
     3|    4     5     7
MAIN: rcv event, t=9.069, at 2 src: 1, dest: 2, contents:   1   0   1   8
                via
   D2 |    0     1    3
  ----|-----------------
     0|    3     2     7
dest 1|    4     1     5
     3|   10     9     2
MAIN: rcv event, t=9.320, at 0 src: 3, dest: 0, contents:   4   3   2   0
                via
   D0 |    1     2    3
  ----|-----------------
     1|    1     4    10
dest 2|    2     3     9
     3|    4     5     7
MAIN: rcv event, t=11.022, at 2 src: 0, dest: 2, contents:   0   1   2   7
                via
   D2 |    0     1    3
  ----|-----------------
     0|    3     2     7
dest 1|    4     1     5
     3|   10     9     2
MAIN: rcv event, t=12.792, at 2 src: 1, dest: 2, contents:   1   0   1   3
                via
   D2 |    0     1    3
  ----|-----------------
     0|    3     2     7
dest 1|    4     1     5
     3|   10     4     2
MAIN: rcv event, t=13.977, at 2 src: 0, dest: 2, contents:   0   1   2   5
                via
   D2 |    0     1    3
  ----|-----------------
     0|    3     2     7
dest 1|    4     1     5
     3|    8     4     2
MAIN: rcv event, t=15.855, at 2 src: 3, dest: 2, contents:   4   3   2   0
                via
   D2 |    0     1    3
  ----|-----------------
     0|    3     2     6
dest 1|    4     1     5
     3|    8     4     2
MAIN: rcv event, t=17.443, at 2 src: 0, dest: 2, contents:   0   1   2   4
                via
   D2 |    0     1    3
  ----|-----------------
     0|    3     2     6
dest 1|    4     1     5
     3|    7     4     2

Simulator terminated at t=17.443218, no packets in medium
```
