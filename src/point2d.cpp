#include "point2d.hpp"
#include <math.h>

namespace librecube {

  point2d::point2d():
    x(0), y(0) {

  }

  point2d::point2d(double x, double y):
    x(x), y(y) {

  }

  point2d::~point2d() {

  }

  const double point2d::get_x() const {

    return this->x;
  }

  const double point2d::get_y() const {

    return this->y;
  }

  void point2d::set_x(double x) {

    this->x = x;
  }

  void point2d::set_y(double y) {

    this->y = y;
  }


  void point2d::set(double x, double y) {

    this->x = x;
    this->y = y;
  }

  double point2d::distance_square (const point2d& point) {

    double dx = point.x - this->x;
    double dy = point.y - this->y;

    return pow(dx, 2) + pow (dy, 2);
  }

  double point2d::distance (const point2d& point) {

    return sqrt(distance_square(point));
  }

}
