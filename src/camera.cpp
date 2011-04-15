#include "camera.hpp"
#include <cmath>

namespace librecube {
  namespace graphics {

    const double PI = 3.14159265358979323846; // PI from <cmath> is not portable

    const vector camera::up(0, 1, 0);

    /**
     * @brief Creates a camera at (0, 0, 0) and looking towards the x axis.
     */
    camera::camera():
        position(0, 0, 0),
        horizontal_angle(0),
        vertical_angle(0) {

      recompute();
    }

    /**
     * @brief Destructor.
     */
    camera::~camera() {

    }

    /**
     * @brief Returns the current position of the camera in the world.
     */
    const vector& camera::get_position() const {

      return position;
    }

    /**
     * @brief Returns a vector of norm 1 corresponding to the direction where the camera is looking.
     */
    const vector& camera::get_looking_direction() const {

      return forward;
    }

    /**
     * @brief Returns some coordinates in the direction where the camera is looking.
     *
     * This function is equivalent to get_position() + get_looking_direction().
     */
    const vector& camera::get_target() const {

      return target;
    }

    /**
     * @brief Sets the position of the camera in the world.
     * @param position coordinates of the new position
     */
    void camera::set_position(const vector& position) {

      this->position = position;
      recompute();
    }

    /**
     * @brief Moves the camera towards its left.
     * @param distance distance of the movement
     */
    void camera::move_left(double distance) {

      position += distance * left;
      recompute();
    }

    /**
     * @brief Moves the camera towards its right.
     * @param distance distance of the movement
     */
    void camera::move_right(double distance) {

      move_left(-distance);
    }

    /**
     * @brief Moves the camera upwards.
     * @param distance distance of the movement
     */
    void camera::move_up(double distance) {
      // TODO
    }

    /**
     * @brief Moves the camera downwards.
     * @param distance distance of the movement
     */
    void camera::move_down(double distance) {
      // TODO
    }

    /**
     * @brief Moves the camera straight ahead.
     * @param distance distance of the movement
     */
    void camera::move_forward(double distance) {

      position += distance * forward;
      recompute();
    }

    /**
     * @brief Moves the camera backwards.
     * @param distance distance of the movement
     */
    void camera::move_backward(double distance) {

      move_forward(-distance);
    }

    /**
     * @brief Changes the horizontal angle to look to the left.
     * @param angle the angle change in radians
     */
    void camera::look_left(double angle) {

      look_right(-angle);
    }

    /**
     * @brief Changes the horizontal angle to look to the right.
     * @param angle the angle change in radians
     */
    void camera::look_right(double angle) {

      horizontal_angle -= angle;
      recompute();
    }

    /**
     * @brief Changes the vertical angle to look upwards.
     * @param angle the angle change in radians
     */
    void camera::look_up(double angle) {

      vertical_angle += angle;
      recompute();
    }

    /**
     * @brief Changes the vertical angle to look downwards.
     * @param angle the angle change in radians
     */
    void camera::look_down(double angle) {

      look_up(-angle);
    }

    /**
     * @brief Recomputes the internal vectors forward, left and target
     * knowing the position and the angles of the camera.
     */
    void camera::recompute() {

      double theta = horizontal_angle * (PI / 180);
      double phi = vertical_angle * (PI / 180);
      double cos_phi = std::cos(phi);

      forward = vector(
          cos_phi * std::cos(theta),
          std::sin(phi),
          -cos_phi * std::sin(theta));

      left = up.cross_product(forward);
      left.normalize();
      target = position + forward;
    }

    /**
     * @brief Prints a string representation of a camera on a stream.
     * @param out an output stream
     * @param cam the camera
     */
    std::ostream& operator<<(std::ostream& out, const camera& cam) {

      out << "position: " << cam.position << ", forward: " << cam.forward;
      return out;
    }

  }
}
