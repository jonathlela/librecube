#ifndef LIBRECUBE_POINT2D_HPP
#define LIBRECUBE_POINT2D_HPP

namespace librecube {
 
  class point2d {

    private:
    
      double x, y;

    public:

      point2d(); 
      point2d(double x, double y);
      ~point2d();

      const double get_x() const;
      const double get_y() const;
      void set_x(double y);
      void set_y(double x);
      void set(double x, double y);
      double distance_square(const point2d& point);
      double distance(const point2d& point);

  };

}

#endif
 
