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
} dt0;


int src_node = 0;
int wts0[4] = {0, 1, 3, 7}; // direct costs to neighbors

#define INF 999

int min_macro0(int a, int b)
{
    return (a < b) ? a : b;
}

int min_cost0(int* a)
{
    return min_macro0(min_macro0(a[0], a[1]), min_macro0(a[2], a[3]));
}

// Send the distance vector to all directly connected neighbors
// min_costs[i] = min_cost(dt0.costs[i]);
// i.e. For destinations marked on columns, minimum cost to reach them
// from node 0 is the minimum of the costs to reach them via all other nodes
int min_costs0[4];
void make_distance_vector()
{
    for (int i = 0; i < 4; i++)
        min_costs0[i] = min_cost0(dt0.costs[i]);
}

void send_packet0()
{
    struct rtpkt packet;
    packet.sourceid = src_node;
    for (int j = 0; j < 4; j++)
        packet.mincost[j] = min_costs0[j];

    for (int i = 0; i < 4; i++)
    {
        if (i == src_node) continue;
        packet.destid = i;
        tolayer2(packet);
    }
}

/* students to write the following two routines, and maybe some others */

void rtinit0() 
{
    // distance_table initialization
    // dt0.costs[i][j] = currently computed cost to node i via direct neighbor j for Node 0
    // dt0.costs[i][i] = wts0[i] for direct neighbors
    // dt0.costs[i][j] = INF for non-direct neighbors
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j) dt0.costs[i][j] = wts0[i];    
            else dt0.costs[i][j] = INF;
        }
    }

    printdt0(&dt0);

    make_distance_vector();
    send_packet0();
}

void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
    int src_node = rcvdpkt->sourceid;

    int changed = 0;
    int old_min_costs[4];
    for (int i = 0; i < 4; i++)
        old_min_costs[i] = min_costs0[i];
    
    // Update the distance table for node 0
    // i.e. if node 0's own minimum cost to another node changes as a result of the update
    // node 0 informs its directly connected neighbors of this change in minimum cost
    // hence node 0's own minimum cost to another node src_node = dt0.costs[src_node][src_node] + min_costs_from_dst[i]

    for (int i = 0; i < 4; i++)
    {
        int new_cost = dt0.costs[src_node][src_node] + rcvdpkt->mincost[i];
        if (new_cost < INF)
            dt0.costs[i][src_node] = new_cost;
        else
            dt0.costs[i][src_node] = INF;
    }
    printdt0(&dt0);

    make_distance_vector();
    for (int i = 0; i < 4; i++)
    {
        if (min_costs0[i] != old_min_costs[i])
        {
            changed = 1;
            break;
        }
    }
    
    if (changed)
        send_packet0();
}


printdt0(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
	 dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
	 dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
	 dtptr->costs[3][2],dtptr->costs[3][3]);
}

linkhandler0(linkid, newcost)   
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}