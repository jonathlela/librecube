#ifndef LIBRECUBE_WORLD_HPP
#define LIBRECUBE_WORLD_HPP

#include "camera.hpp"
#include "block.hpp"
#include <SFML/Window.hpp>

namespace librecube {

  /**
   * @brief Represents the LibreCube world.
   */
  class world {

    private:

      camera cam;
      block test_block;

    public:

      world();
      ~world();

      void process_event(sf::Event& event);
      void update();
      void draw();
  };
}

#endif
