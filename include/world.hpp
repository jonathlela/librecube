#ifndef LIBRECUBE_WORLD_HPP
#define LIBRECUBE_WORLD_HPP

#include "camera.hpp"
#include "block.hpp"
#include "avatar.hpp"
#include "individual.hpp"
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
        network::individual ind;
        block test_block;

      public:

        world();
        ~world();

        void process_event(sf::Event& event);
        void update();
        void draw();
    };

  }
}

#endif
