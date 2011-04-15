#ifndef LIBRECUBE_VECTOR_HPP
#define LIBRECUBE_VECTOR_HPP

#include <cmath>
#include <iostream>

namespace librecube {

  /**
   * @brief A 3D point or vector.
   */
  class vector {

    private:

      double x;
      double y;
      double z;

    public:

      vector();
      vector(const vector& other);
      vector(int x, int y, int z);
      vector(double x, double y, double z);
      ~vector();

      double get_x() const;
      void set_x(double x);

      double get_y() const;
      void set_y(double y);

      double get_z() const;
      void set_z(double z);

      vector& operator=(const vector& other);
      vector operator+(const vector& other) const;
      vector& operator+=(const vector& other);
      vector operator-(const vector& other) const;
      vector& operator-=(const vector& other);
      friend vector operator*(double scale, vector& v);
      friend vector operator*(vector& v, double scale);
      friend vector& operator*=(vector& v, double scale);
      friend std::ostream& operator<<(std::ostream& out, const vector& v);

      vector cross_product(const vector& other) const;
      void normalize();
  };

  /**
   * @brief Creates a point with coordinates (0, 0, 0).
   */
  inline vector::vector():
      x(0),
      y(0),
      z(0) {

  }

  /**
   * @brief Copy constructor.
   * @param other a point to copy
   */
  inline vector::vector(const vector& other):
      x(other.x),
      y(other.y),
      z(other.z) {

  }

  /**
   * @brief Creates a point with the specified integer coordinates.
   * @param x x coordinate
   * @param y y coordinate
   * @param z z coordinate
   */
  inline vector::vector(int x, int y, int z):
      x(x),
      y(y),
      z(z) {

  }

  /**
   * @brief Creates a point with the specified real coordinates.
   * @param x x coordinate
   * @param y y coordinate
   * @param z z coordinate
   */
  inline vector::vector(double x, double y, double z):
      x(x),
      y(y),
      z(z) {

  }

  /**
   * @brief Destructor.
   */
  inline vector::~vector() {

  }

  /**
   * @brief Assignment operator.
   * @param other the point to copy
   */
  inline vector& vector::operator=(const vector& other) {

    x = other.x;
    y = other.y;
    z = other.z;

    return *this;
  }

  inline vector vector::operator+(const vector& other) const {

    vector result(*this);
    result += other;

    return result;
  }

  inline vector& vector::operator+=(const vector& other) {

    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
  }

  inline vector vector::operator-(const vector& other) const {

    vector result(*this);
    result -= other;

    return result;
  }

  inline vector& vector::operator-=(const vector& other) {

    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
  }

  inline vector operator*(const vector& v, double scale) {

    vector result(v);
    result *= scale;

    return result;
  }

  inline vector operator*(double scale, vector& v) {

    vector result(v);
    result *= scale;

    return result;
  }

  inline vector& operator*=(vector& v, double scale) {

    v.x *= scale;
    v.y *= scale;
    v.z *= scale;

    return v;
  }

  inline double vector::get_x() const {

    return x;
  }

  inline void vector::set_x(double x) {

    this->x = x;
  }

  inline double vector::get_y() const {

    return y;
  }

  inline void vector::set_y(double y) {

    this->y = y;
  }

  inline double vector::get_z() const {

    return z;
  }

  inline void vector::set_z(double z) {

    this->z = z;
  }

  inline std::ostream& operator<<(std::ostream& out, const vector& v) {

    out << "(" << v.x << "," << v.y << "," << v.z << ")";
    return out;
  }
}

#endif
