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
        static const unsigned char MSG_GET_NODE;
        static const unsigned char MSG_GET_NEIGHBORS;
        static const unsigned char MSG_SET_NEIGHBORS;
        static const unsigned char MSG_GET_ADDRESSES;
        static const unsigned char MSG_SET_ADDRESSES;
        static const unsigned char MSG_GET_SUBSCRIPTION;
        static const unsigned char MSG_SET_SUBSCRIPTION;
      
        static const std::string UNASSIGNED_ID;

        sf::IPAddress address;
        unsigned short port;

        node site;

        bool is_up;
        std::set<node *> neighbors;
        std::map<std::string, std::pair <sf::IPAddress ,unsigned short> > addresses;

        sf::SelectorTCP selector;
        sf::SocketTCP server;

        std::stringstream logname;
        std::ofstream log;

        sf::Clock clock;

        virtual void Run();
        bool receive_message(sf::SocketTCP *socket);
        void send_message(const unsigned char type,
			  sf::Packet packet_content, 
			  sf::SocketTCP *socket);

        void send_message(const unsigned char type, 
			  sf::Packet packet_content, 
			  const sf::IPAddress gateway,
			  const unsigned short port);

        friend sf::Packet& operator<<(sf::Packet& packet, const std::pair <sf::IPAddress ,unsigned short>& pair);
        friend sf::Packet& operator>>(sf::Packet& packet, std::pair <sf::IPAddress ,unsigned short>& pair);
        friend sf::Packet& operator<<(sf::Packet& packet,
				      const std::map<char *, std::pair <sf::IPAddress ,unsigned short> >& map);
        friend sf::Packet& operator>>(sf::Packet& packet, 
				      std::map<char *, std::pair <sf::IPAddress ,unsigned short> >& map);
        friend sf::Packet& operator<<(sf::Packet& packet, const std::set<node *>& node);
        friend sf::Packet& operator>>(sf::Packet& packet, std::set<node *>& node);

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
