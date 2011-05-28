#include "line2d.hpp"
#include <math.h>

namespace librecube {

  line2d::line2d():
    p1(), p2(), a(0), b(0), c(0) {

  }

  line2d::line2d(point2d& p1, point2d& p2):
    p1(p1), p2(p2) {

    compute_triplet(p1.get_x(), p1.get_y(), p2.get_x(), p2.get_y());
  }

  line2d::line2d(double x1, double y1, double x2, double y2):
    p1(x1, y1), p2(x2, y2) {

    compute_triplet(x1, y1, x2, y2);
  }

  line2d::line2d(double a, double b, double c):
    p1(), p2(), a(a), b(b), c(c) {

  }

  line2d::~line2d() {

  }

  point2d line2d::get_p1() {

    return this->p1;
  }

  point2d line2d::get_p2() {

    return this->p2;
  }

  double line2d::get_a() {

    return this->a;
  }

  double line2d::get_b() {

    return this->b;
  }


  double line2d::get_c() {

    return this->c;
  }

  void line2d::compute_triplet(double x1, double y1, double x2, double y2) {

    this->a = y2 - y1;
    this->b = x1 - x2;
    this->c = this->a*x1 + this->b*y1;
  }

  bool line2d::intersection(line2d& line, point2d& point) {

    double D;
    bool res;

    D = (this->a * line.get_b()) - (this->b * line.get_a());
    if (D == 0) {
      point.set_x(0);
      point.set_y(0);
      res = false;
    }
    else {
      point.set_x((this->c * line.get_b() - this->b * line.get_c()) / D / 1.0);
      point.set_y((this->a * line.get_c() - this->c * line.get_a()) / D / 1.0);
      res = true;
    }
    return res;
  }

  double line2d::distance(const point2d& point) {
    return fabs(this->a * point.get_x() + this->b * point.get_y() + this->c)
      / sqrt (pow(this->a, 2) + pow(this->b, 2));
    }

}

