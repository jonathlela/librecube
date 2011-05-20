#include "peer.hpp"
#include <SFML/System.hpp>
#include <sys/time.h>
#include <string>

namespace librecube {
  namespace network {

    const unsigned short peer::DEFAULT_PORT = 5436;

    const unsigned char peer::MSG_GET_ID = 0;
    const unsigned char peer::MSG_SET_ID = 1;
    const unsigned char peer::MSG_GET_NODE = 2;
    const unsigned char peer::MSG_GET_NEIGHBORS = 3;
    const unsigned char peer::MSG_SET_NEIGHBORS = 4;
    const unsigned char peer::MSG_GET_ADDRESSES = 5;
    const unsigned char peer::MSG_SET_ADDRESSES = 6;
    const unsigned char peer::MSG_GET_SUBSCRIPTION = 7;
    const unsigned char peer::MSG_SET_SUBSCRIPTION = 8;

    const std::string peer::UNASSIGNED_ID = "UNASSIGNED";

    /**
     * @brief Creates a peer
     */
    peer::peer():
      site(),
      is_up(false),
      neighbors(), 
      addresses() {

      Launch();
      while(!is_up) {usleep(10);}
    }


    /**
     * @brief Destructor.
     */
    peer::~peer() {
      is_up = false;
      Wait();
    }

     /**
     * @brief Returns the peer id.
     */
    const std::string peer::get_id() const{

      return site.get_id();
    }

    /**
     * @brief Returns the peer position.
     */
    const vector peer::get_position() const{

      return site.get_position();
    }

    /**
     * @brief Sets the peer position.
     */
    void peer::set_position(const vector& position) {

      this->site.set_position(position);
    }

    sf::Packet& operator<<(sf::Packet& packet, const std::set<node *>& set) {

      std::set<node *>::iterator it;      

      packet << ((int) set.size());
      for (it=set.begin(); it!=set.end(); it++) {
	packet << *it;
      }      
      return packet;
    }
    
    sf::Packet& operator>>(sf::Packet& packet, std::set<node *>& set) {

      int size;

      packet >> size;
      for (int i=0; i < size ;i++) {
	node new_site;
	packet >> new_site;
	set.insert(&new_site);
      }
      return packet;
    }

    sf::Packet& operator<<(sf::Packet& packet, const std::pair<sf::IPAddress ,unsigned short>& pair) {
      
      packet << pair.first.ToInteger() << pair.second;
      return packet;
    }

    sf::Packet& operator>>(sf::Packet& packet, std::pair<sf::IPAddress ,unsigned short>& pair) {
      
      int new_address;
      unsigned short new_port;

      packet >> new_address >> new_port;
      pair.first = sf::IPAddress(new_address);
      pair.second = new_port;
      return packet;
    }
 
    sf::Packet& operator<<(sf::Packet& packet,
			   const std::map<std::string, std::pair<sf::IPAddress ,unsigned short> >& map) {

      std::map<std::string, std::pair<sf::IPAddress ,unsigned short> >::const_iterator  it;      

      packet << ((int) map.size());
      for (it=map.begin(); it!=map.end(); it++) {
	packet << it->first << it->second;
      }      
      return packet;
    }
    
    sf::Packet& operator>>(sf::Packet& packet, 
			   std::map<std::string, std::pair<sf::IPAddress ,unsigned short> >& map) {
      
      int size;

      packet >> size;
      for (int i=0; i < size ;i++) {
	std::string new_id;
	std::pair <sf::IPAddress ,unsigned short>  new_address;
	packet >> new_id >> new_address;
	map.insert(std::make_pair(new_id,new_address)); 
      }
      return packet;
    }

    void peer::send_message(const unsigned char type,
				  sf::Packet packet_content, 
				  sf::SocketTCP *socket) {

     sf::Packet packet_type;
     sf::Packet packet_id;

     packet_type << type;
     socket->Send(packet_type);

     packet_id << site.get_id();
     socket->Send(packet_id);

     socket->Send(packet_content);
    }

    void peer::send_message(const unsigned char type, 
				  sf::Packet packet_content, 
				  const sf::IPAddress gateway,
				  const unsigned short port) {
      sf::SocketTCP client;
 
      client.Connect(port, gateway);
      send_message(type, packet_content, &client);
    }

    bool peer::receive_message(sf::SocketTCP *socket) {
      
      sf::Packet packet_type;
      sf::Packet packet_id;
      sf::Packet packet_content;
      bool status = true;

      status = (socket->Receive(packet_type) == sf::Socket::Done
		&& socket->Receive(packet_id) == sf::Socket::Done);

      if (status) {
	  unsigned char type;
	  std::string p_id;

	  packet_type >> type;
	  packet_id >> p_id;

	  log << "type : " << ((int) type) << ", id: "<< p_id << std::endl;

	  switch( type ) {
	    
	  case MSG_GET_ID : {
	    std::stringstream n_id;
	    timeval time = {0, 0};
	    sf::Packet id_packet;

	    status = socket->Receive(packet_content) == sf::Socket::Done;
	    gettimeofday(&time, NULL);
	    n_id << time.tv_sec << time.tv_usec << address.ToInteger();
	    log << "id_get : " << n_id.str() << std::endl;   
	    id_packet << n_id.str();
	    send_message(MSG_SET_ID, id_packet, socket);
	    log << "sent " << std::endl;   
	  }
	    break;

	  case MSG_SET_ID : {
	    std::string n_id;
	    
	    status = socket->Receive(packet_content) == sf::Socket::Done;
	    packet_content >> n_id;
	    log << "id_set : " << n_id << std::endl;
	    site.set_id(n_id);
	  }
	    break;

	  case MSG_GET_ADDRESSES : {
	    sf::Packet address_packet;
	    std::map<std::string, std::pair <sf::IPAddress ,unsigned short> >::iterator it;

	    status = socket->Receive(packet_content) == sf::Socket::Done;
	    
	    log << "addresses :";
	    for (it=addresses.begin(); it!=addresses.end(); it++) {
	      log << it->first << "=" << it->second.first.ToInteger() << ":" << it->second.second << " ";
	    }
	    log << std::endl;
	    address_packet << addresses;
	    send_message(MSG_SET_ADDRESSES, address_packet, socket);
	    log << "sent " << std::endl;   
	  }
	    break;

	  case MSG_SET_ADDRESSES : {
	    std::map<std::string, std::pair <sf::IPAddress ,unsigned short> > new_addresses;
	    std::map<std::string, std::pair <sf::IPAddress ,unsigned short> >::iterator it;
	    
	    status = socket->Receive(packet_content) == sf::Socket::Done;
	    packet_content >> new_addresses;
	    log << "set addresses :";
	    for (it=new_addresses.begin(); it!=new_addresses.end(); it++) {
	      log << it->first << "=" << it->second.first.ToInteger() << ":" << it->second.second << " ";
	      addresses.insert(std::make_pair(it->first, it->second));
	    }
	    log << std::endl;
	    log << "addresses set : " << std::endl;
	  }
	    break;	    

	  case MSG_GET_NODE : {
	    sf::Packet node_packet;
    	    std::set<node *> curr_site;

	    curr_site.insert(&site);
	    status = socket->Receive(packet_content) == sf::Socket::Done;
	    log << "node_get : " << std::endl;   
	    node_packet << curr_site;
	    send_message(MSG_SET_NEIGHBORS, node_packet, socket);
	    log << "sent " << std::endl;
	    
	  }
	    break;
	    
	  case MSG_GET_NEIGHBORS : {
	    sf::Packet neigbors_packet;

	    status = socket->Receive(packet_content) == sf::Socket::Done;
	    log << "neigbors_get : " << std::endl;   
	    neigbors_packet << neighbors;
	    send_message(MSG_SET_NEIGHBORS, neigbors_packet, socket);
	    log << "sent " << neigbors_packet << "…" << std::endl;
	    
	  }
	    break;
	    
	  case MSG_SET_NEIGHBORS : {
	    std::set<node *> new_neighbors;
	    std::set<node *>::iterator it;

	    status = socket->Receive(packet_content) == sf::Socket::Done;
	    packet_content >> new_neighbors;
	    
	    log << "neighbors ";
	    for (it=neighbors.begin(); it!=neighbors.end(); it++) {
	      log << *it;
	    }
	    log << std::endl;
	    log << "added_neigbors ";
	    for (it=new_neighbors.begin(); it!=new_neighbors.end(); it++) {
	      log << "a" << *it;
	    }
	    log << std::endl;
 	    for (it=new_neighbors.begin(); it!=new_neighbors.end(); it++) {
	      if ((*it)->get_id() != site.get_id())
		log << "b" << *it;
	    }
	    log << "neighbors ";
	    for (it=neighbors.begin(); it!=neighbors.end(); it++) {
	      log << "c" << *it;
	    }
	    log << std::endl;
	  }
	    break;

	  case MSG_GET_SUBSCRIPTION : {
	    std::stringstream n_id;
	    timeval time = {0, 0};
	    sf::Packet id_packet;

	    status = socket->Receive(packet_content) == sf::Socket::Done;
	    gettimeofday(&time, NULL);
	    n_id << time.tv_sec << time.tv_usec << address.ToInteger();
	    log << "id_get : " << n_id.str() << std::endl;   
	    id_packet << n_id.str();
	    send_message(MSG_SET_SUBSCRIPTION, id_packet, socket);
	    log << "sent " << std::endl;   
	  }
	    break;

	  case MSG_SET_SUBSCRIPTION : {
	    std::string n_id;
	    
	    status = socket->Receive(packet_content) == sf::Socket::Done;
	    packet_content >> n_id;
	    log << "id_set : " << n_id << std::endl;
	    site.set_id(n_id);
	  }
	    break;

	      
	  default:
	    log << "??" << std::endl;
	    status = false;
	    break;
	  }
      } else {
	log << "remove" << std::endl;      
	selector.Remove(*socket);
      }
      return status;	  
    }

    /**
     * @brief Run the listening message part of a peer
     */
    void peer::Run() {

      bool running = false;
      site.set_id(UNASSIGNED_ID);
      port = DEFAULT_PORT;
      address = sf::IPAddress::GetLocalAddress();

      // Create a successfull listening socket
      while (!running) {
	running = true;
	if (!server.Listen(port)) {
	  log << "cannot listen port " << port << std::endl;
	  running = false;
	  port = port + 1;
	}
      }
      logname << "log" << port << ".txt";
      log.open(logname.str().c_str());

      selector.Add(server);

      is_up = true;
      
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
	      receive_message(&socket);
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
    void peer::join_gateway(const sf::IPAddress gateway, const unsigned short port) {

      sf::Packet packet_content;
      sf::SocketTCP client;
 
      client.Connect(port, gateway);
      send_message(MSG_GET_ID, packet_content, &client); 
      receive_message(&client);
      log << "given id" << site.get_id() << std::endl;
      send_message(MSG_GET_NEIGHBORS, packet_content, &client);
      receive_message(&client);
    }

    /**
     * @brief Leave from the network
     */
    void peer::leave() {
      
    }

    /**
     * @brief Prints a std::string representation of a peer on a stream.
     * @param out an output stream
     * @param cam the peer
     */
    std::ostream& operator<<(std::ostream& out, const peer& peer) {

      out << "peer: " << peer.site.get_id();
      return out;
    }

 
  }
}
