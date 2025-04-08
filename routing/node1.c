#include <stdio.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt1;

void printdt1(struct distance_table *dtptr)
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}

int src_node = 1;
int wts1[4] = {1, 0, 1, 999}; // direct costs to neighbors

#define INF 999

int min_macro1(int a, int b)
{
    return (a < b) ? a : b;
}

int min_cost1(int* a)
{
    return min_macro1(min_macro1(a[0], a[1]), min_macro1(a[2], a[3]));
}

int min_costs1[4];
void make_distance_vector1()
{
    for (int i = 0; i < 4; i++)
        min_costs1[i] = min_cost1(dt1.costs[i]);
}

void send_packet1()
{
    struct rtpkt packet;
    packet.sourceid = src_node;
    for (int j = 0; j < 4; j++)
        packet.mincost[j] = min_costs1[j];

    for (int i = 0; i < 4; i++)
    {
        if (i == src_node) continue;
        packet.destid = i;
        tolayer2(packet);
    }
}

/* students to write the following two routines, and maybe some others */


void rtinit1() 
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j) dt1.costs[i][j] = wts1[i];    
            else dt1.costs[i][j] = INF;
        }
    }

    if (TRACE > 0) printdt1(&dt1);

    make_distance_vector1();
    send_packet1();
}


void rtupdate1(struct rtpkt *rcvdpkt)
{
    int src_node = rcvdpkt->sourceid;

    int changed = 0;
    int old_min_costs[4];
    for (int i = 0; i < 4; i++)
        old_min_costs[i] = min_costs1[i];
    
        for (int i = 0; i < 4; i++)
        {
            int new_cost = dt1.costs[src_node][src_node] + rcvdpkt->mincost[i];
            if (new_cost < INF)
                dt1.costs[i][src_node] = new_cost;
            else
                dt1.costs[i][src_node] = INF;
        }
        if (TRACE > 0) printdt1(&dt1);
    
        make_distance_vector1();
        for (int i = 0; i < 4; i++)
        {
            if (min_costs1[i] != old_min_costs[i])
            {
                changed = 1;
                break;
            }
        }
        
        if (changed)
            send_packet1();
}

void linkhandler1(int linkid, int newcost)   
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}