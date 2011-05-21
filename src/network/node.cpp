#include "node.hpp"
#include <SFML/System.hpp>

namespace librecube {
  namespace network {

    sf::Mutex global_mutex;

    const unsigned short node::RADIUS = 16;
    const std::string node::UNASSIGNED_ID = "UNASSIGNED";

    /**
     * @brief Creates a node
     */
    node::node():
      id(UNASSIGNED_ID),
      position() {

    }

    /**
     * @brief Destructor.
     */
    node::~node() {

    }

    /**
     * @brief Returns the node id.
     */
    const std::string node::get_id() const{

      return id;
    }

    /**
     * @brief Sets the node id
     */
    void node::set_id(std::string id) {

      this->id = id;
    }

    /**
     * @brief Returns the node position.
     */
    const vector node::get_position() const{

      return position;
    }

    /**
     * @brief Sets the node position.
     */
    void node::set_position(const vector& position) {

      sf::Lock Lock(global_mutex);  
      this->position = position;
    }

    /**
     * @brief Tells if a point is in node aoi
     * @param point the point
     */
    const bool node::is_point_in_aoi(const vector& point) const{
      
      if (point.get_x() > position.get_x() + RADIUS 
	  || point.get_x() < position.get_x() - RADIUS)
	return false;
      if (point.get_y() > position.get_y() + RADIUS 
	  || point.get_y() < position.get_y() - RADIUS)
	return false;
      if (point.get_z() > position.get_z() + RADIUS 
	  || point.get_z() < position.get_z() - RADIUS)
	return false;
      return true;
    }

    /**
     * @brief Prints a representation of a node on a packet.
     * @param packet an output packet
     * @param node the node
     */
    sf::Packet& operator<<(sf::Packet& packet, const node& node)
    {
      packet << node.id << node.position.get_x() << node.position.get_y() << node.position.get_z();
      return packet;
    }

    /**
     * @brief Gets a node on from a packet.
     * @param packet an input packet
     * @param node the node
     */    
    sf::Packet& operator>>(sf::Packet& packet, node& node)
    {
      double x, y, z;
      packet >> node.id >> x >> y >> z;
      node.position = vector(x, y, z);
      return packet;
    }

    bool node::operator<(const node& node) const {
      return (this->id < node.id);
    }

    /**
     * @brief Prints a string representation of a node on a stream.
     * @param out an output stream
     * @param cam the node
     */
    std::ostream& operator<<(std::ostream& out, const node& node) {

      out << "node: " << node.id << " " << node.position.get_x() << "," << node.position.get_y() << "," << node.position.get_z();
      return out;
    }

 
  }
}
