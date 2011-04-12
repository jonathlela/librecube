#ifndef LIBRECUBE_CAMERA_HPP
#define LIBRECUBE_CAMERA_HPP

#include "vector.hpp"
#include <iostream>

namespace librecube {

  /**
   * @brief The player's point of view.
   */
  class camera {

    private:

      static const vector up;       // the (0, 1, 0) vector

      vector position;              // current coordinates of the camera
      vector forward;               // vector from the camera to where it
                                    // looks (of norm 1)
      vector left;                  // vector of norm 1 from the camera to
                                    // its left
      vector target;                // coordinates where the camera is
                                    // looking (i.e. position + forward)

      double horizontal_angle;      // current horizontal angle of the camera
                                    // (relative to the x axis)
      double vertical_angle;        // current vertical angle of the camera
                                    // (relative to the x axis)

      void recompute();

    public:

      camera();
      ~camera();

      const vector& get_position() const;
      const vector& get_looking_direction() const;
      const vector& get_target() const;

      void set_position(const vector& position);
      void move_left(double distance);
      void move_right(double distance);
      void move_up(double distance);
      void move_down(double distance);
      void move_forward(double distance);
      void move_backward(double distance);
      void look_left(double angle);
      void look_right(double angle);
      void look_up(double angle);
      void look_down(double angle);

      friend std::ostream& operator<<(std::ostream& out, const camera& camera);
  };
}

#endif

