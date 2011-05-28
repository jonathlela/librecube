#ifndef LIBRECUBE_LINE2D_HPP
#define LIBRECUBE_LINE2D_HPP

#include "point2d.hpp"


namespace librecube {

  class line2d {

    protected:

      point2d p1, p2;
      double a, b, c;

    private:

      void compute_triplet(double x1, double y1, double x2, double y2);

    public:

      line2d();
      line2d(point2d& p1, point2d& p2);
      line2d(double x1, double x2, double y1, double y2);
      line2d(double a, double b, double c);
      ~line2d();

      point2d get_p1();
      point2d get_p2();
      double get_a();
      double get_b();
      double get_c();

      double distance(const point2d& point);
      bool intersection(line2d& line, point2d& point);


  };

}

#endif
