#include <stdio.h>
#include "routing.h"

// extern struct rtpkt {
//   int sourceid;       /* id of sending router sending this pkt */
//   int destid;         /* id of router to which pkt being sent 
//                          (must be an immediate neighbor) */
//   int mincost[4];    /* min cost to node 0 ... 3 */
//   };

// extern int TRACE;
// extern int YES;
// extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt3;

void printdt3(struct distance_table *dtptr)
{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}

int wts3[4] = {7, 999, 2, 0}; // direct costs to neighbors

#define INF 999

int min_macro3(int a, int b)
{
    return (a < b) ? a : b;
}

int min_cost3(int* a)
{
    return min_macro3(min_macro3(a[0], a[1]), min_macro3(a[2], a[3]));
}

int min_costs3[4];
void make_distance_vector3()
{
    for (int i = 0; i < 4; i++)
        min_costs3[i] = min_cost3(dt3.costs[i]);
}

void send_packet3()
{
    struct rtpkt packet;
    packet.sourceid = 3;
    for (int j = 0; j < 4; j++)
        packet.mincost[j] = min_costs3[j];

    for (int i = 0; i < 4; i++)
    {
        if (i == 3 || i == 1) continue;
        packet.destid = i;
        tolayer2(packet);
    }
}


/* students to write the following two routines, and maybe some others */

void rtinit3() 
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j) dt3.costs[i][j] = wts3[i];    
            else dt3.costs[i][j] = INF;
        }
    }

    if (TRACE > 0) printdt3(&dt3);

    make_distance_vector3();
    send_packet3();
}


void rtupdate3(struct rtpkt *rcvdpkt)
{
    int src = rcvdpkt->sourceid;

    int changed = 0;
    int old_min_costs[4];
    for (int i = 0; i < 4; i++)
        old_min_costs[i] = min_costs3[i];

    for (int i = 0; i < 4; i++)
    {
        int new_cost = dt3.costs[src][src] + rcvdpkt->mincost[i];
        if (new_cost < INF)
            dt3.costs[i][src] = new_cost;
        else
            dt3.costs[i][src] = INF;
    }
    if (TRACE > 0) printdt3(&dt3);

    make_distance_vector3();
    for (int i = 0; i < 4; i++)
    {
        if (min_costs3[i] != old_min_costs[i])
        {
            changed = 1;
            break;
        }
    }
    
    if (changed)
        send_packet3();
}