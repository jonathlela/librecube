#ifndef LIBRECUBE_NODE_HPP
#define LIBRECUBE_NODE_HPP

#include "vector.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <string>

namespace librecube {
  namespace network {

    /**
     * @brief The player in the network
     */
    class node {

      private:

        std::string id;
        vector position;
      
      public:

        static const unsigned short RADIUS;
        static const std::string UNASSIGNED_ID;

        node();
        ~node();

        const std::string get_id() const;
        void set_id(std::string id);

        const vector get_position() const;
        void set_position(const vector& position);

        const bool is_point_in_aoi(const vector& point) const;

        friend sf::Packet& operator<<(sf::Packet& packet, const node& node);
        friend sf::Packet& operator>>(sf::Packet& packet, node& node);

        bool operator<(const node& node) const;
        bool operator==(const node& node) const;

        friend std::ostream& operator<<(std::ostream& out, const node& node);
    };

  }
}

#endif
