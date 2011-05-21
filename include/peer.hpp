#ifndef LIBRECUBE_PEER_HPP
#define LIBRECUBE_PEER_HPP

#include "vector.hpp"
#include "node.hpp"
#include <SFML/Network.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <string>

namespace librecube {
  namespace network {

    /**
     * @brief The player in the network
     */
    class peer : private sf::Thread {

      private:

        static const unsigned short DEFAULT_PORT;

        static const unsigned char MSG_GET_ID;
        static const unsigned char MSG_SET_ID;
        static const unsigned char MSG_GET_ADDRESS;
        static const unsigned char MSG_SET_ADDRESS;
        static const unsigned char MSG_GET_NEIGHBORS;
        static const unsigned char MSG_SET_NEIGHBORS;
        static const unsigned char MSG_GET_ADDRESSES;
        static const unsigned char MSG_SET_ADDRESSES;
        static const unsigned char MSG_GET_SUBSCRIPTION;
        static const unsigned char MSG_SET_SUBSCRIPTION;
        static const unsigned char MSG_PUBLISH_SUBSCRIPTION;
        static const unsigned char MSG_PUBLISH_POSITION;
      
        sf::IPAddress address;
        unsigned short port;

        node site;

        bool is_up;
        std::set<node> neighbors;
        std::map<std::string, std::pair <sf::IPAddress ,unsigned short> > addresses;

        sf::SelectorTCP selector;
        sf::SocketTCP server;

        std::stringstream logname;
        std::ofstream log;

        sf::Clock clock;

        void add_neighbor(const node& node);
        void add_neighbors(const std::set<node>& nodes);
        void add_address(const std::string id,
			 const std::pair <sf::IPAddress ,unsigned short> address) ;
	void add_addresses(const std::map<std::string, std::pair <sf::IPAddress ,unsigned short> > addresses);

        virtual void Run();
        bool receive_message(sf::SocketTCP *socket);
        bool send_message(const unsigned char type,
			  sf::Packet& content, 
			  sf::SocketTCP *socket);
        bool send_message(const unsigned char type, 
			  sf::Packet& content, 
			  const sf::IPAddress gateway,
			  const unsigned short port);
        bool send_message(const unsigned char type, 
			  sf::Packet& packet, 
			  const std::string to_id);

        bool msg_get_id(sf::SocketTCP *socket,
			const std::string& from_id, 
			sf::Packet& packet);
        bool msg_get_address(sf::SocketTCP *socket,
			const std::string& from_id, 
			sf::Packet& packet);
        bool msg_get_neighbors(sf::SocketTCP *socket,
			const std::string& from_id, 
			sf::Packet& packet);
        bool msg_get_addresses(sf::SocketTCP *socket,
			const std::string& from_id, 
			sf::Packet& packet);
        bool msg_get_subscription(sf::SocketTCP *socket,
			const std::string& from_id, 
			sf::Packet& packet);
        bool msg_publish_subscription(sf::SocketTCP *socket,
			const std::string& from_id, 
			sf::Packet& packet);

        long long get_time();

        bool msg_receive(sf::SocketTCP *socket, std::string& from_id, sf::Packet& packet); 
        bool msg_receive_id(sf::SocketTCP *socket,
			    std::string& id);  
        bool msg_receive_address(sf::SocketTCP *socket,
				 std::pair <sf::IPAddress ,unsigned short>& address);
        bool msg_receive_neighbors(sf::SocketTCP *socket,
				   std::set<node>& neighbors);
        bool msg_receive_addresses(sf::SocketTCP *socket,
				   std::map<std::string, std::pair <sf::IPAddress ,unsigned short> >& addresses);
        bool msg_receive_subscription(sf::SocketTCP *socket,
			    std::string& id);  

        friend sf::Packet& operator<<(sf::Packet& packet, const std::set<node>& node);
        friend sf::Packet& operator>>(sf::Packet& packet, std::set<node>& node);
        friend sf::Packet& operator<<(sf::Packet& packet, sf::IPAddress& ip);
        friend sf::Packet& operator>>(sf::Packet& packet, sf::IPAddress& ip);
        friend sf::Packet& operator<<(sf::Packet& packet, const std::pair <sf::IPAddress ,unsigned short>& pair);
        friend sf::Packet& operator>>(sf::Packet& packet, std::pair <sf::IPAddress ,unsigned short>& pair);
        friend sf::Packet& operator<<(sf::Packet& packet,
				      const std::map<std::string, std::pair <sf::IPAddress ,unsigned short> >& map);
        friend sf::Packet& operator>>(sf::Packet& packet, 
				      std::map<std::string, std::pair <sf::IPAddress ,unsigned short> >& map);

      public:

        peer();
        ~peer();

      const std::string get_id() const;

        const vector get_position() const;
        void set_position(const vector& position);

        void join_gateway(const sf::IPAddress gateway, const unsigned short port);
	void leave();

        friend std::ostream& operator<<(std::ostream& out, const peer& peer);
    };

  }
}

#endif
