#include "ace/ACE.h"
#include "VASTVerse.h"
#include "VASTsim.h"
#include <iostream>
#include <pthread.h>

 
#define VAST_EVENT_LAYER    1                   // layer ID for sending events
#define VAST_UPDATE_LAYER   2                   // layer ID for sending updates
 
using namespace Vast;
using namespace std;
 
int main (int argc, char *argv[])
{   VASTPara_Net    netpara   (VAST_NET_ACE);              // network parameters
    NodeState       state1     = ABSENT;   // the join state of this node
    NodeState       state2     = ABSENT;   // the join state of this node
    Area            aoi1;                  // my AOI (with center as current position)
    Area            aoi2;                  // my AOI (with center as current position)
    Addr            gateway;              // address for gateway
 
    VASTVerse *     world      = NULL;
    VAST *          self1      = NULL;
    VAST *          self2      = NULL;
    world_t     world_id = 0;
    int         node_no1 = (-1);
    int         node_no2 = (-1); 
    Vast::id_t            sub_no1    = 0;        // subscription # for my client (peer)
    Vast::id_t            sub_no2    = 0;        // subscription # for my client (peer)

    int             input;                // keyboard input
    int interval = 0;
    // store default gateway address
    char str[] = "127.0.0.1:1037";

    bool is_gateway1;
    bool is_gateway2;
    SimPara simpara;
    cout << str << endl;
 
    if ((node_no1 = InitPara (VAST_NET_ACE, netpara, simpara, "", &is_gateway1, &world_id, &aoi1, str, &interval)) == (-1))
      exit (0);

    if ((node_no2 = InitPara (VAST_NET_ACE, netpara, simpara, "", &is_gateway2, &world_id, &aoi2, str, &interval)) == (-1))
      exit (0);

    // create VAST node factory
    world = new VASTVerse (is_gateway1, str, &netpara, NULL, NULL, 40);
    world->createVASTNode (world_id, aoi1, VAST_EVENT_LAYER);

 
    bool finished = false;
 
    // record beginning of main loop
    while (!finished)
    {
        if (state1 != JOINED)
        {
            if ((self1 = world->getVASTNode ()) != NULL)
            {
                sub_no1 = self1->getSubscriptionID ();
                state1 = JOINED;
            }
        }
	else if (state2 != JOINED)
        {
            if ((self2 = world->getVASTNode ()) != NULL)
            {
                sub_no2 = self2->getSubscriptionID ();
                state2 = JOINED;
            }
        }
        else
        {
            // obtain input from user, will store inside 'aoi'
            //input = cin.get();
	    //cout << "input" << input << endl;
            //self1->move (sub_no1, aoi1);
        }
 
        // perform some per-second tasks
 
        // process incoming message and perform routine tasks
        world->tick ();
 
        // sleep a little if there's unused time
	sleep (1);
    }
 
    // depart
    self1->leave ();
    world->destroyVASTNode (self1);
 
    delete world;
 
    return 0;
}
