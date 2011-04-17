#include "ace/ACE.h"
#include "VASTVerse.h"
#include <iostream>
#include <pthread.h>

#define VAST_EVENT_LAYER    1                   // layer ID for sending events
#define VAST_UPDATE_LAYER   2                   // layer ID for sending updates

#define DEFAULT_AOI         195     // the AOI-radius size for all nodes
#define DIM_X               768     // size of the world & its default values
#define DIM_Y               768

#define INPUT_SIZE          200                 // size for chat message
 
using namespace Vast;
using namespace std;
 
int main (int argc, char *argv[])
{   VASTPara_Net    netpara   (VAST_NET_ACE);              // network parameters
    NodeState       state     = ABSENT;   // the join state of this node
 
    Area            aoi;                  // my AOI (with center as current position)
  
    Addr            gateway;              // address for gateway
 
    VASTVerse *     world      = NULL;
    VAST *          self      = NULL;

    world_t     world_id = 0;
    int         node_no = 0;

    int tick = 0;
 
    Vast::id_t            sub_no    = 0;        // subscription # for my client (peer)

    int             input;                // keyboard input
    int interval = 0;
 
    bool is_gateway = true;

    char str[] = "127.0.0.1:1037";

    // set default values
    aoi.center.x = (coord_t)(rand () % DIM_X);
    aoi.center.y = (coord_t)(rand () % DIM_Y);
    aoi.radius   = (length_t)DEFAULT_AOI;

    netpara.port  = GATEWAY_DEFAULT_PORT;

    netpara.relay_limit     = 0;
    netpara.client_limit    = 0;
    netpara.overload_limit  = 0;

    // by default the node can be relay & matcher
    netpara.is_relay = true;
    netpara.is_matcher = true;

    // dynamic load balancing is enabled by default
    netpara.is_static = false;

    netpara.is_entry = true;

    // create VAST node factory
    world = new VASTVerse (is_gateway, str, &netpara, NULL, NULL, 40);
    world->createVASTNode (world_id, aoi, VAST_EVENT_LAYER);

    bool finished = false;
 
    // record beginning of main loop
    while (!finished)
    {
        if (state != JOINED)
        {
            if ((self = world->getVASTNode ()) != NULL)
            {
                sub_no = self->getSubscriptionID ();
                state = JOINED;
            }
        }
        else
        {
	  char recv_buf[INPUT_SIZE];
	  size_t size = 0;

	  Message *msg_receive = NULL;
	  Message msg_send (12);

	  char helloworld [50];
	  snprintf(helloworld, sizeof(helloworld), "%i %i hello world", sub_no, tick);

	  do
	    {       
	      if ((msg_receive = self->receive ()) != NULL)
		{
		  size = msg_receive->extract (recv_buf, 0);
		  recv_buf[size]=0;
		}
	      else
		size = 0;

	      if (size > 0)
		{
		  string chatmsg (recv_buf, size);
		  cout << "receive: " << chatmsg << endl;
		}
	    }
	  while (size > 0);
     	  
	  msg_send.store (helloworld, strlen(helloworld), true);
	  
	  self->publish (aoi, VAST_EVENT_LAYER, msg_send);
	}
 
	tick = tick + 1;
        // perform some per-second tasks
 
        // process incoming message and perform routine tasks
        world->tick ();
 
        // sleep a little if there's unused time
	sleep (1);
    }
 
    // depart
    self->leave ();
    world->destroyVASTNode (self);
 
    delete world;
 
    return 0;
}
