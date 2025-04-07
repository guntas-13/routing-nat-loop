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


/* students to write the following two routines, and maybe some others */

void rtinit0() 
{
    /*
    It should initialize the distance table in node 0 to reflect 
    the direct costs of 1, 3, and 7 to nodes 1, 2, and 3, respectively. 
    All links are bi-directional and the costs in both directions are identical. 
    After initializing the distance table, and any other data structures needed 
    by your node 0 routines, it should then send its directly-connected neighbors 
    (in this case, 1, 2 and 3) the cost of it minimum cost paths, 
    i.e., its distance vector, to all other network nodes. 
    This minimum cost information is sent to neighboring nodes in a routing packet 
    by calling the routine tolayer2()
    */
    
    int src_node = 0;
    int direct_costs[4] = {0, 1, 3, 7};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            dt0.costs[i][j] = 999; // Initialize to a large value
            if (i == src_node)
                dt0.costs[i][j] = direct_costs[j];
            else if (i == j)
                dt0.costs[i][j] = 0; 
                
        }
    }
    
    printdt0(&dt0);

    struct rtpkt packet;
    packet.sourceid = 0;
    for (int i = 0; i < 4; i++)
    {
        if (i == src_node) continue;
        packet.destid = i;
        packet.mincost[0] = dt0.costs[0][0];
        packet.mincost[1] = dt0.costs[0][1];
        packet.mincost[2] = dt0.costs[0][2];
        packet.mincost[3] = dt0.costs[0][3];
        tolayer2(packet);
    }
}


void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{

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