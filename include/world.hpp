#ifndef LIBRECUBE_WORLD_HPP
#define LIBRECUBE_WORLD_HPP

#include <SFML/Window.hpp>

namespace librecube {

  /**
   * @brief Represents the LibreCube world.
   */
  class world {

    public:

      world();
      ~world();

      void process_event(sf::Event& event);
      void update();
      void draw();
  };
}

#endif
