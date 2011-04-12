#include "vector.hpp"

namespace librecube {

  void vector::normalize() {

    double norm = std::sqrt(x * x + y * y + z * z);
    x /= norm;
    y /= norm;
    z /= norm;
  }

  vector vector::cross_product(const vector& other) const {

    return vector(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x);
  }
}
