#ifndef LIBRECUBE_LIBRECUBE_HPP
#define LIBRECUBE_LIBRECUBE_HPP

#include <SFML/Window.hpp>
#include "world.hpp"

namespace librecube {

  /**
   * @brief Main class.
   */
  class librecube {

    private:

      sf::Window window;     // the SFML window
      world game;            // the game logics

      void process_event(sf::Event& event);
      void update();
      void draw();

    public:

      librecube();
      ~librecube();

      void main_loop();
  };
}

#endif
