#include "peer.hpp"
#include <SFML/System.hpp>
#include <sys/time.h>
#include <string>

namespace librecube {
  namespace network {
    
    sf::Mutex global_mutex;
    
    const unsigned short peer::DEFAULT_PORT = 5436;
    
    const unsigned char peer::MSG_GET_SITE = 0;
    const unsigned char peer::MSG_SET_SITE = 1;
    const unsigned char peer::MSG_GET_ADDRESS = 2;
    const unsigned char peer::MSG_SET_ADDRESS = 3;
    const unsigned char peer::MSG_GET_NEIGHBORS = 4;
    const unsigned char peer::MSG_SET_NEIGHBORS = 5;
    const unsigned char peer::MSG_GET_ADDRESSES = 6;
    const unsigned char peer::MSG_SET_ADDRESSES = 7;
    const unsigned char peer::MSG_GET_SUBSCRIPTION = 8;
    const unsigned char peer::MSG_SET_SUBSCRIPTION = 9;
    const unsigned char peer::MSG_PUBLISH_SUBSCRIPTION = 10;
    const unsigned char peer::MSG_PUBLISH_POSITION = 11;
    const unsigned char peer::MSG_PUBLISH_CHAT = 12;
    
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
      
      sf::Packet packet;
      std::map<std::string, node>::iterator it;

      this->site.set_position(position);
      compute_aoi_neighbors();
      packet << position;
      for (it=this->neighbors.begin(); it!=this->neighbors.end(); it++) {
	send_message(MSG_PUBLISH_POSITION, packet, it->first); 
      }      
    }

    const std::map<std::string, node> peer::get_aoi_neighbors() const {
      
      return this->aoi_neighbors;
    }
    
    void peer::add_neighbor(const node& node) {
      
      sf::Lock Lock(global_mutex);  
      if (node.get_id() != this->site.get_id() && node.get_id() != node::UNASSIGNED_ID) {
	this->neighbors[node.get_id()] = node;
	compute_aoi_neighbors();
      }
    }
    
    void peer::add_neighbors(const std::map<std::string, node>& nodes) {
      
      std::map<std::string, node>::const_iterator it;
      
      sf::Lock Lock(global_mutex);  
      for (it=nodes.begin(); it!=nodes.end(); it++) {
	if (it->first != this->site.get_id() && it->first != node::UNASSIGNED_ID)
	  this->neighbors[it->first] = it->second;
      }
      compute_aoi_neighbors();
    }
    
    void peer::add_address(const std::string& id,
			   const std::pair <sf::IPAddress ,unsigned short>& address) {
      
      sf::Lock Lock(global_mutex);
      if (id != this->site.get_id() && id != node::UNASSIGNED_ID)
	this->addresses[id] = address;
    }
    
    void peer::add_addresses(const std::map<std::string, std::pair <sf::IPAddress ,unsigned short> >& addresses) {
      
      std::map<std::string, std::pair <sf::IPAddress ,unsigned short> >::const_iterator it;
      
      sf::Lock Lock(global_mutex);
      for (it=addresses.begin(); it!=addresses.end(); it++) {
	if (it->first != this->site.get_id() && it->first != node::UNASSIGNED_ID)
	  this->addresses[it->first] = it->second;
      }
    }
    
    sf::Packet& operator<<(sf::Packet& packet, const std::map<std::string, node>& nodes) {
      
      std::map<std::string, node>::const_iterator it;      
      
      packet << ((int) nodes.size());
      for (it=nodes.begin(); it!=nodes.end(); it++) {
	packet << it->first << it->second;
      }      
      return packet;
    }
    
    sf::Packet& operator>>(sf::Packet& packet, std::map<std::string, node>& nodes) {
      
      int size;
      
      packet >> size;
      for (int i=0; i < size ;i++) {
	std::string new_id;
	node new_site;
	packet >> new_id >> new_site;
	nodes[new_id] = new_site;
      }
      return packet;
    }
    
    sf::Packet& operator<<(sf::Packet& packet, const sf::IPAddress& ip) {
      
      packet << ip.ToInteger();
      return packet;
    }
    
    
    sf::Packet& operator>>(sf::Packet& packet, sf::IPAddress& ip) {
      
      uint32_t new_ip;
      
      packet >> new_ip;
      ip = sf::IPAddress(new_ip);
      return packet;
    }
    
    sf::Packet& operator<<(sf::Packet& packet, const std::pair<sf::IPAddress ,unsigned short>& pair) {
      
      packet << pair.first << pair.second;
      return packet;
    }
    
    sf::Packet& operator>>(sf::Packet& packet, std::pair<sf::IPAddress ,unsigned short>& pair) {
      
      int new_address;
      unsigned short new_port;
      
      packet >> new_address >> new_port;
      pair.first = new_address;
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
    
    bool peer::send_message(const unsigned char type,
			    sf::Packet& content, 
			    sf::SocketTCP *socket) {
      
      sf::Packet packet;
      
      packet << type << site.get_id();
      packet.Append(content.GetData(), content.GetDataSize());
      
      return socket->Send(packet);
    }
    
    bool peer::send_message(const unsigned char type, 
			    sf::Packet& content, 
			    const sf::IPAddress gateway,
			    const unsigned short port) {
      sf::SocketTCP client;
      
      client.Connect(port, gateway);
      return send_message(type, content, &client);
    }
    
    
    bool peer::send_message(const unsigned char type, 
			    sf::Packet& packet, 
			    const std::string to_id) {
      
      std::map<std::string, std::pair<sf::IPAddress ,unsigned short> >::iterator it;
      
      it = addresses.find(to_id);
      if (it!=addresses.end())
	return send_message(type, packet, it->second.first, it->second.second);
      return false;
    }
    
    bool peer::msg_get_site(sf::SocketTCP *socket, const std::string& from_id, 
    			  sf::Packet& packet) {
      
      sf::Packet content;
      
      content << site;
      return send_message(MSG_SET_SITE, content, socket);	  
    }
    
    bool peer::msg_get_address(sf::SocketTCP *socket, const std::string& from_id, 
			       sf::Packet& packet) {
      
      sf::Packet content;
      
      content << std::make_pair(this->address, this->port);
      return send_message(MSG_SET_ADDRESS, content, socket);	  
    }
    
    
    bool peer::msg_get_neighbors(sf::SocketTCP *socket, const std::string& from_id, 
				 sf::Packet& packet) {
      
      sf::Packet content;
      
      content << this->neighbors;
      send_message(MSG_SET_NEIGHBORS, content, socket);
    }
    
    
    bool peer::msg_get_addresses(sf::SocketTCP *socket, const std::string& from_id, 
				 sf::Packet& packet) {
      
      sf::Packet content;
      
      content << this->addresses;
      send_message(MSG_SET_ADDRESSES, content, socket);
    }
    
    
    bool peer::msg_get_subscription(sf::SocketTCP *socket, const std::string& from_id, 
				    sf::Packet& packet) {
      
      sf::Packet content;
      std::stringstream n_id;
      timeval time = {0, 0};
      
      gettimeofday(&time, NULL);
      n_id << time.tv_sec << time.tv_usec << address.ToInteger();
      content << n_id.str();    
      return send_message(MSG_SET_SUBSCRIPTION, content, socket);     
    }
    
    
    bool peer::msg_publish_subscription(sf::SocketTCP *socket, const std::string& from_id, 
					sf::Packet& packet) {
      
      node new_node;
      sf::IPAddress new_address;
      unsigned short new_port;
      
      packet >> new_node >> new_address >> new_port;
      add_neighbor(new_node);
      add_address(new_node.get_id(),std::make_pair(new_address,new_port));
      log << "pub set :" << new_node.get_id() << " " << new_address << ":"  << new_port << std::endl;
      return true;
    }
    
    bool peer::msg_publish_position(sf::SocketTCP *socket, const std::string& from_id, 
				    sf::Packet& packet) {
      
      vector position;
      node site;

      packet >> position;
      site.set_id(from_id);
      site.set_position(position);
      add_neighbor(site);
      
      return true;
    }

    bool peer::msg_publish_chat(sf::SocketTCP *socket, const std::string& from_id, 
				sf::Packet& packet) {
      
      std::string msg;
      
      packet >> msg;
      std::cout << msg << std::endl;
      return true;
    }
    

    bool peer::msg_receive(sf::SocketTCP *socket, std::string& from_id, sf::Packet& packet) {
      
      bool status = true;
      unsigned short type;
      
      status = (socket->Receive(packet) == sf::Socket::Done);
      if (status) {
	unsigned char type;
	std::string p_id;
	packet >> type >> from_id;
      }
      return status;
    }
    
    bool peer::msg_receive_site(sf::SocketTCP *socket, node& node) {
      
      sf::Packet packet;
      std::string from_id;
      bool status = true;
      
      status = msg_receive(socket, from_id, packet);
      packet >> node;
      log << "node_set: " << node << std::endl;   
      return status;
    }
    
    bool peer::msg_receive_address(sf::SocketTCP *socket, std::pair<sf::IPAddress ,unsigned short>& address) {
      
      sf::Packet packet;
      std::string from_id;
      int ip;
      bool status = true;
      
      status = msg_receive(socket, from_id, packet);
      packet >> address;
      log << "address_set: " << address.first.ToString() << ":" << address.second << std::endl;   
      return status;
    }
    
    bool peer::msg_receive_neighbors(sf::SocketTCP *socket, std::map<std::string, node>& neighbors) {
      
      sf::Packet packet;
      std::string from_id;
      std::map<std::string, node>::iterator it;
      bool status = true;
      
      status = msg_receive(socket, from_id, packet);
      packet >> neighbors;
      log << "neighbors_set: ";
      for (it=neighbors.begin(); it!=neighbors.end(); it++) {
	log << it->first << ", " << it->second << " ";
      }
      log << std::endl;
      return status;
    }
    
    bool peer::msg_receive_addresses(sf::SocketTCP *socket,
				     std::map<std::string, std::pair<sf::IPAddress ,unsigned short> >& addresses) {
      
      sf::Packet packet;
      std::string from_id;
      std::map<std::string, std::pair <sf::IPAddress ,unsigned short> >::iterator it;
      
      bool status = true;
      
      status = msg_receive(socket, from_id, packet);	    
      packet >> addresses;
      log << "addresses set: ";
      for (it=addresses.begin(); it!=addresses.end(); it++) {
	log << it->first << ", " << it->second.first.ToInteger() << ":" << it->second.second << " ";
      }
      log << std::endl;
      return status;      
    }
    
    bool peer::msg_receive_subscription(sf::SocketTCP *socket, std::string& id) {
      
      sf::Packet packet;
      std::string from_id;
      bool status = true;
      
      status = msg_receive(socket, from_id, packet);
      packet >> id;
      log << "subscription_set : " << id  << std::endl;   
      return status;
    }
    
    long long peer::get_time() {
      std::stringstream n_id;
      timeval time = {0, 0};
      
      gettimeofday(&time, NULL);
      return time.tv_sec * 1000000 + time.tv_usec;
    }
    
    bool peer::receive_message(sf::SocketTCP *socket) {
      
      sf::Packet content;
      bool status = true;

      status = (socket->Receive(content) == sf::Socket::Done);
      
      if (status) {
	unsigned char type;
	std::string p_id;
	
	content >> type >> p_id;
	log << "@type : " << ((int) type) << ", id: "<< p_id << std::endl;
	
	switch( type ) {
	  
	case MSG_GET_SITE : {
	  msg_get_site(socket, p_id, content);
	  }
	  break;
	  
	case MSG_GET_ADDRESS : {
	  msg_get_address(socket, p_id, content);
	}
	  break;
	  
	case MSG_GET_ADDRESSES : {
	  msg_get_addresses(socket, p_id, content);
	}
	  break;
	  
	case MSG_GET_NEIGHBORS : {
	  msg_get_neighbors (socket, p_id, content);
	}
	  break;

	case MSG_GET_SUBSCRIPTION : {
	  msg_get_subscription(socket, p_id, content);
	}
	  break;
	  
	case MSG_PUBLISH_SUBSCRIPTION : {
	  msg_publish_subscription(socket, p_id, content);
	}
	  break;

	case MSG_PUBLISH_POSITION : {
	  msg_publish_position(socket, p_id, content);
	}
	  break;

	case MSG_PUBLISH_CHAT : {
	  msg_publish_chat(socket, p_id, content);
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
	
	for (unsigned int i = 0; i < nb_sockets; ++i) {
	  
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
      std::string given_id;
      node gat;
      std::pair<sf::IPAddress ,unsigned short> gat_ip;
      std::map<std::string, node> new_neighbors;
      std::map<std::string, node>::iterator it;
      std::map<std::string, std::pair <sf::IPAddress ,unsigned short> > new_addresses;
      
      
      client.Connect(port, gateway);
      send_message(MSG_GET_SUBSCRIPTION, packet_content, &client); 
      msg_receive_subscription(&client, given_id);
      site.set_id(given_id);
      send_message(MSG_GET_SITE, packet_content, &client); 
      msg_receive_site(&client, gat);
      add_neighbor(gat);
      send_message(MSG_GET_ADDRESS, packet_content, &client);
      msg_receive_address(&client, gat_ip);
      add_address(gat.get_id(), gat_ip);
      send_message(MSG_GET_NEIGHBORS, packet_content, &client); 
      msg_receive_neighbors(&client, new_neighbors);
      add_neighbors(new_neighbors);
      send_message(MSG_GET_ADDRESSES, packet_content, &client); 
      msg_receive_addresses(&client, new_addresses);
      add_addresses(new_addresses);
      packet_content << this->site << this->address << this->port;
      for (it=this->neighbors.begin(); it!=this->neighbors.end(); it++) {
	send_message(MSG_PUBLISH_SUBSCRIPTION, packet_content, it->first); 
      }
    }

    /**
     * @brief Leave from the network
     */
    void peer::leave() {
      
    }

    void peer::compute_aoi_neighbors() {

      std::map<std::string, node>::iterator it;

      this->aoi_neighbors.clear();
      for (it=this->neighbors.begin(); it!=this->neighbors.end(); it++) {
	if (this->site.is_point_in_aoi(it->second.get_position()))
	  this->aoi_neighbors[it->first] =  it->second;	
      }
    }

    void peer::send_msg(const std::string& msg) {
           
      sf::Packet packet;
      std::map<std::string, node>::iterator it;

      packet << msg;
      for (it=this->aoi_neighbors.begin(); it!=this->aoi_neighbors.end(); it++) {
	send_message(MSG_PUBLISH_CHAT, packet, it->first);
      }
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
