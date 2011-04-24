#ifndef LIBRECUBE_WORLD_HPP
#define LIBRECUBE_WORLD_HPP

#include "camera.hpp"
#include "block.hpp"
#include "avatar.hpp"
#include "individual.hpp"
#include "VASTVerse.h"
#include <map>
#include <SFML/Window.hpp>

namespace librecube {
  namespace graphics {

    /**
     * @brief Represents the LibreCube world.
     */
    class world {

      private:

        camera cam;
        avatar ava;
        std::map<const Vast::id_t, avatar> oth_avatars;
        network::individual ind;
        block test_block;
	int* blocs;

      public:

        world();
        ~world();

        void process_event(sf::Event& event);
        void update();
        void draw();
    };

  }
  int* generate_world();
}

#endif
