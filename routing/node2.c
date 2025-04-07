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
} dt2;


/* students to write the following two routines, and maybe some others */

void rtinit2() 
{
    int src_node = 2;
    int direct_costs[4] = {3, 1, 0, 2};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            dt2.costs[i][j] = 999; // Initialize to a large value
            if (i == src_node)
                dt2.costs[i][j] = direct_costs[j];
            else if (i == j)
                dt2.costs[i][j] = 0; 
                
        }
    }
    
    printdt2(&dt2);

    struct rtpkt packet;
    packet.sourceid = src_node;
    for (int i = 0; i < 4; i++)
    {
        if (i == src_node) continue;
        packet.destid = i;
        packet.mincost[0] = dt2.costs[0][0];
        packet.mincost[1] = dt2.costs[0][1];
        packet.mincost[2] = dt2.costs[0][2];
        packet.mincost[3] = dt2.costs[0][3];
        tolayer2(packet);
    }
}


void rtupdate2(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{

}


printdt2(dtptr)
  struct distance_table *dtptr;
  
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