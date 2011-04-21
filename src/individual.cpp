#include "individual.hpp"

using namespace Vast;

namespace librecube {
  namespace network {

    const length_t individual::radius = 16;
    const msgtype_t individual::msgtype = 12;
    const layer_t individual::event_layer = 1;
    const layer_t individual::chat_layer = 3;

    /**
     * @brief Creates a individual
     */
    individual::individual():
      aoi (Position(),radius),
      netpara (VAST_NET_ACE) {

      bool is_gateway = true;
      char adr[] = "127.0.0.1:1037";

      state = ABSENT;
      world = NULL;
      self = NULL;
      world_id = VAST_DEFAULT_WORLD_ID;
      node_no = 0;
      sub_no = 0;

      netpara.port = GATEWAY_DEFAULT_PORT;

      netpara.relay_limit = 0;
      netpara.client_limit = 0;
      netpara.overload_limit = 0;

      // by default the node can be relay & matcher
      netpara.is_relay = true;
      netpara.is_matcher = true;

      // dynamic load balancing is enabled by default
      netpara.is_static = false;

      netpara.is_entry = true;

      // create VAST node factory
      world = new VASTVerse (is_gateway, adr, &netpara, NULL, NULL, 40);
      world->createVASTNode (world_id, aoi, event_layer);

      // Join the network
      while (state != JOINED)
	{
	  if ((self = world->getVASTNode ()) != NULL)
	    {
	      sub_no = self->getSubscriptionID ();
	      state = JOINED;
	    }
      
	  world->tick ();
	}

    }

    /**
     * @brief Destructor.
     */
    individual::~individual() {

      self->leave ();
      world->destroyVASTNode (self);
 
      delete world;
    }

    /**
     * @brief Returns the current position of the individual in the world.
     */
    const Position& individual::get_map_position() const{

      return aoi.center;
    }

    /**
     * @brief Sets the position of the idnividual in the world.
     * @param position coordinates of the new position
     */
    void individual::set_map_position(const Position& position) {

      this->aoi.center = position;
      this->self->move (this->sub_no, this->aoi);
    }

   /**
     * @brief Get the individual id
     */
    const Vast::id_t individual::get_id() const{
      this->sub_no;
    }

    /**
     * @brief Sets the position of the individual in the world.
     * @param position coordinates of the new position
     */
    void individual::set_map_position(const vector& position) {

      this->aoi.center.set((coord_t)position.get_x(), (coord_t)position.get_z(), (coord_t)position.get_y());
      this->self->move (this->sub_no, this->aoi);
    }

    /**
     * @brief Looks for new messages
     */
    void individual::receive_msg() {

      char recv_buf[200];
      size_t size = 0;
      Message *msg_receive = NULL;

      do
	{       
	  if ((msg_receive = this->self->receive ()) != NULL)
	    {
	      size = msg_receive->extract (recv_buf, 0);
	      recv_buf[size]=0;
	    }
	  else
	    size = 0;

	  if (size > 0)
	    {
	      string chatmsg (recv_buf, size);
	      std::cout << "receive: " << chatmsg << endl;
	    }
	}
      while (size > 0);	  
    }

    /**
     * @brief Sends an helloworld message to the peers
     */
    void individual::send_helloworld() {
      Message msg_send (msgtype);
      char helloworld [50];

      snprintf(helloworld, sizeof(helloworld), "%i hello world", sub_no);
      msg_send.store (helloworld, strlen(helloworld), true);
      this->self->publish (this->aoi, event_layer, msg_send);
    }

    /**
     * @brief Gets neighbours
     */
    std::vector<Node *> individual::get_neighbors() {
      return this->self->list ();
    }

    /**
     * @brief Performs routine tasks
     */
    void individual::tick() {
      this->world->tick ();
    }

   /**
     * @brief Prints a string representation of a individual on a stream.
     * @param out an output stream
     * @param cam the individual
     */
    std::ostream& operator<<(std::ostream& out, const individual& individual) {

      out << "position: (" << individual.aoi.center.x << ", " << individual.aoi.center.z << ", " << individual.aoi.center.y <<  "), radius: " << individual.aoi.radius;
      return out;
    }

  }
}
