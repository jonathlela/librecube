#include "node.hpp"
#include <fstream>
#include <sstream>

namespace librecube {
  namespace network {

    const unsigned short node::radius = 16;
    const unsigned short node::port = 5436;

    /**
     * @brief Creates a node
     */
    node::node():
      id(0),
      is_up(true),
      position(),
      neighbors() {

      Launch();
    }

    /**
     * @brief Destructor.
     */
    node::~node() {
      is_up = false;
      Wait();
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

      this->position = position;
    }

    /**
     * @brief Run the listening message part of a node
     */
    void node::Run() {

      sf::SelectorTCP selector;
      sf::SocketTCP server;
      bool running = false;
      unsigned short actual_port = port;
      std::stringstream logname;
      std::ofstream log;

      // Create a successfull listening socket
      while (!running) {
	running = true;
	if (!server.Listen(actual_port)) {
	  log << "cannot listen port " << actual_port << std::endl;
	  running = false;
	  actual_port = actual_port + 1;
	}
      }
      logname << "log" << actual_port << ".txt";
      log.open(logname.str().c_str());

      selector.Add(server);

      while (is_up) {

	unsigned int nb_sockets = selector.Wait(0.1f);

	log << nb_sockets << std::endl; 

	for (unsigned int i = 0; i < nb_sockets; ++i) {

	  log << "socket:" << i << std::endl;
	  
	  sf::SocketTCP socket = selector.GetSocketReady(i);
	  
	  if (socket == server) {
	    // If the listening socket is ready, it means that we can accept a new connection
	    sf::IPAddress address;
	    sf::SocketTCP client;
	    server.Accept(client, &address);
	    log << "Client connected ! (" << address << ")" << std::endl;
	    
	    // Add it to the selector
	    selector.Add(client);
	  } else {
	    // Else, it is a client socket so we can read the data he sent
	    sf::Packet packet;
	    if (socket.Receive(packet) == sf::Socket::Done) {        
	      // Extract the message and display it
	      std::string message;
	      packet >> message;
	      log << "A client says : \"" << message << "\"" << std::endl;
	    } else {
	      // Error : we'd better remove the socket from the selector
	      selector.Remove(socket);
	    }   
	  }
	}
      }
      log.close();
    }
    

    /**
     * @brief Join network by a gateway
     * @param gateway the ip address of the gateway
     * @param port the port of the gateway
     */
    void node::join_gateway(const sf::IPAddress gateway, const int port) {

      sf::SocketTCP client;

      if (!client.Connect(port, gateway)) {
	return;
      }

      std::string message ("ok");

      sf::Packet packet;
      packet << message;
      client.Send(packet);
      
    }

    /**
     * @brief Leave from the network
     */
    void node::leave() {
      
    }

    /**
     * @brief Tells if a point is in node aoi
     * @param point the point
     */
    const bool node::is_point_in_aoi(const vector& point) const{
      
      if (point.get_x() > position.get_x() + radius 
	  || point.get_x() < position.get_x() - radius)
	return false;
      if (point.get_y() > position.get_y() + radius 
	  || point.get_y() < position.get_y() - radius)
	return false;
      if (point.get_z() > position.get_z() + radius 
	  || point.get_z() < position.get_z() - radius)
	return false;
      return true;
    }

    /**
     * @brief Prints a string representation of a node on a stream.
     * @param out an output stream
     * @param cam the node
     */
    std::ostream& operator<<(std::ostream& out, const node& node) {

      out << "node: " << node.id;
      return out;
    }

 
  }
}
