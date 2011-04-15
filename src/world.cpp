#include "world.hpp"

namespace librecube {
  namespace graphics {

    world::world() {

      cam.set_position(vector(-20, 10, 2));
    }

    world::~world() {

    }

    void world::process_event(sf::Event& event) {

      if (event.Type == sf::Event::KeyPressed) {

        switch (event.Key.Code) {

          case sf::Key::Z:
            cam.move_forward(1);
            break;

          case sf::Key::S:
            cam.move_backward(1);
            break;

          case sf::Key::D:
            cam.move_right(1);
            break;

          case sf::Key::Q:
            cam.move_left(1);
            break;

          case sf::Key::Right:
            cam.look_right(2);
            break;

          case sf::Key::Left:
            cam.look_left(2);
            break;

          case sf::Key::Up:
            cam.look_up(2);
            break;

          case sf::Key::Down:
            cam.look_down(2);
            break;

          default:
            break;
        }
      }
      else if (event.Type == sf::Event::MouseMoved) {
        // TODO
      }
    }

    void world::update() {

    }

    void world::draw() {

      // look towards the camera's direction
      const vector& position = cam.get_position();
      const vector& target = cam.get_target();
      gluLookAt(
          position.get_x(), position.get_y(), position.get_z(),  // current position
          target.get_x(), target.get_y(), target.get_z(),        // where to look
          0, 1, 0                                                // vertical axis
          );

      // display a block
      vector where;
      for (int i = -8; i < 8; i+=2) {

        where.set_x(i);
        for (int j = -8; j < 8; j+=2) {
          where.set_z(j);
          for (int k = -8; k < 8; k+=2) {
            where.set_y(k);
            test_block.draw(where);
          }
        }
      }
    }
  }
}
