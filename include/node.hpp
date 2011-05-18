#ifndef LIBRECUBE_NODE_HPP
#define LIBRECUBE_NODE_HPP

#include "vector.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>

namespace librecube {
  namespace network {
    /**
     * @brief The player in the network
     */
    class node : private sf::Thread {

      private:

        static const unsigned short radius;
        static const unsigned short port;

        int id;
        bool is_up;
        vector position;
        std::vector<node *> neighbors;

        virtual void Run();

      public:

        node();
        ~node();

        const vector get_position() const;
        void set_position(const vector& position);

        void join_gateway(const sf::IPAddress gateway, const int port);
	void leave();

        const bool is_point_in_aoi(const vector& point) const;

        friend std::ostream& operator<<(std::ostream& out, const node& node);
    };

  }
}

#endif
