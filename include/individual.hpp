#ifndef LIBRECUBE_INDIVIDUAL_HPP
#define LIBRECUBE_INDIVIDUAL_HPP

#include "VASTVerse.h"
#include "vector.hpp"
#include "avatar.hpp"
#include <iostream>


namespace librecube {
  namespace network {

    /**
     * @brief The player in the network
     */
    class individual {

      private:
      
        static const Vast::length_t radius;
        static const Vast::msgtype_t msgtype;
        static const Vast::layer_t event_layer;
        static const Vast::layer_t chat_layer;
      
        Vast::VASTPara_Net netpara;
        Vast::NodeState state;   // the join state of this node
        Vast::VASTVerse * world;
        Vast::VAST * self;
        Vast::world_t world_id;
        int node_no;
        Vast::id_t sub_no;
        Vast::Area aoi;

      public:

        individual();
        ~individual();

        const Vast::Position& get_map_position() const;

        void set_map_position(const Vast::Position& position);
        void set_map_position(const vector& position);

        void receive_msg(void);
        void send_helloworld(void);
        void tick(void);

        friend std::ostream& operator<<(std::ostream& out, const individual& individual);
    };

  }
}

#endif