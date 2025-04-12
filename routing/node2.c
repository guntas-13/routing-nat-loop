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
} dt2;

void printdt2(struct distance_table *dtptr)
{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}

int wts2[4] = {3, 1, 0, 2}; // direct costs to neighbors

#define INF 999

int min_macro2(int a, int b)
{
    return (a < b) ? a : b;
}

int min_cost2(int* a)
{
    return min_macro2(min_macro2(a[0], a[1]), min_macro2(a[2], a[3]));
}

int min_costs2[4];
void make_distance_vector2()
{
    for (int i = 0; i < 4; i++)
        min_costs2[i] = min_cost2(dt2.costs[i]);
}

void send_packet2()
{
    struct rtpkt packet;
    packet.sourceid = 2;
    for (int j = 0; j < 4; j++)
        packet.mincost[j] = min_costs2[j];

    for (int i = 0; i < 4; i++)
    {
        if (i == 2) continue;
        packet.destid = i;
        tolayer2(packet);
    }
}

/* students to write the following two routines, and maybe some others */

void rtinit2() 
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j) dt2.costs[i][j] = wts2[i];    
            else dt2.costs[i][j] = INF;
        }
    }

    if (TRACE > 0) printdt2(&dt2);

    make_distance_vector2();
    send_packet2();
}


void rtupdate2(struct rtpkt *rcvdpkt)
{
    int src = rcvdpkt->sourceid;

    int changed = 0;
    int old_min_costs[4];
    for (int i = 0; i < 4; i++)
        old_min_costs[i] = min_costs2[i];

    for (int i = 0; i < 4; i++)
    {
        int new_cost = dt2.costs[src][src] + rcvdpkt->mincost[i];
        if (new_cost < INF)
            dt2.costs[i][src] = new_cost;
        else
            dt2.costs[i][src] = INF;
    }
    if (TRACE > 0) printdt2(&dt2);

    make_distance_vector2();
    for (int i = 0; i < 4; i++)
    {
        if (min_costs2[i] != old_min_costs[i])
        {
            changed = 1;
            break;
        }
    }
    
    if (changed)
        send_packet2();
}