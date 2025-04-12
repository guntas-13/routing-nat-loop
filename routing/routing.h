#ifndef ROUTING_H
#define ROUTING_H

struct rtpkt {
  int sourceid;
  int destid;
  int mincost[4];
};

void tolayer2(struct rtpkt packet);
void rtinit0();
void rtinit1();
void rtinit2();
void rtinit3();
void rtupdate0(struct rtpkt *rcvdpkt);
void rtupdate1(struct rtpkt *rcvdpkt);
void rtupdate2(struct rtpkt *rcvdpkt);
void rtupdate3(struct rtpkt *rcvdpkt);

void linkhandler0(int linkid, int newcost);
void linkhandler1(int linkid, int newcost);


int TRACE;
int YES;
int NO;

#endif