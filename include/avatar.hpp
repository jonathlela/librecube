#ifndef LIBRECUBE_AVATAR_HPP
#define LIBRECUBE_AVATAR_HPP

#include "vector.hpp"
#include <iostream>

namespace librecube {
  namespace graphics {

    /**
     * @brief The player's point of view.
     */
    class avatar {

      private:

        static const vector up;       // the (0, 1, 0) vector

        vector position;              // current coordinates of the avatar
        vector forward;               // vector from the avatar to where it
                                      // looks (of norm 1)
        vector left;                  // vector of norm 1 from the avatar to
                                      // its left
        vector target;                // coordinates where the avatar is
                                      // looking (i.e. position + forward)

        double horizontal_angle;      // current horizontal angle of the avatar
                                      // (relative to the x axis)
        double vertical_angle;        // current vertical angle of the avatar
                                      // (relative to the x axis)

        void recompute();

      public:

        avatar();
        ~avatar();

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

        friend std::ostream& operator<<(std::ostream& out, const avatar& avatar);
    };

  }
}

#endif
