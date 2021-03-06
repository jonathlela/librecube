#include "librecube.hpp"
#include <iostream>

namespace librecube {

  /**
   * @brief Creates and initializes a LibreCube application.
   */
  librecube::librecube():
      window(sf::VideoMode(800, 600, 32), "LibreCube"),
      game() {

    // initialize OpenGL settings
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glMatrixMode(GL_PROJECTION);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);
  }

  /**
   * @brief Destroys the application.
   */
  librecube::~librecube() {

  }

  /**
   * @brief Runs LibreCube.
   */
  void librecube::main_loop() {

    sf::Clock clock;
    while (window.IsOpened())
    {
      sf::Event event;
      while (window.GetEvent(event)) {
        process_event(event);
      }
      if (window.IsOpened()) {

        update();

        float elapsed_time = clock.GetElapsedTime();
        if (elapsed_time > 0.04) { // 25 FPS
          draw();
          clock.Reset();
        }
        else {
          sf::Sleep(0.01);
        }
      }
    }
  }

  /**
   * @brief Handles an event received in the window.
   */
  void librecube::process_event(sf::Event& event) {

    switch (event.Type) {

      case sf::Event::Closed: // window closed
        window.Close();
        break;

      case sf::Event::Resized: // window resized
        // tell OpenGL to use the whole new window as its viewport
        glViewport(0, 0, event.Size.Width, event.Size.Height);
        break;

      case sf::Event::KeyPressed: // quit button
	if (event.Key.Code == sf::Key::X) {
	  window.Close();
	} else {
	  game.process_event(event);
	}
        break;

      default: // forward any other event to the game
        game.process_event(event);
    }
  }

  /**
   * @brief Updates the game.
   *
   * This function is called repeatedly by the main loop.
   */
  void librecube::update() {
    game.update();
  }

  /**
   * @brief Redraws the screen.
   *
   * This function is called repeatedly by the main loop.
   */
  void librecube::draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    game.draw();

    window.Display();
  }
}

/**
 * @brief Main function.
 *
 * Creates and executes a LibreCube game.
 *
 * @param argc number of command-line arguments
 * @param argv command-line arguments
 */
int main (int argc, char* argv[])
{
  std::cout << "Whoa ! It's already 20% cooler than minecraft !" << std::endl;
  librecube::librecube().main_loop();
  return 0;
}
