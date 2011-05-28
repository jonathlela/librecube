#ifndef LIBRECUBE_EDGE_HPP
#define LIBRECUBE_EDGE_HPP

#include "line2d.hpp"
#include "node.hpp"

namespace librecube {
  namespace network {
    class edge : public librecube::line2d {

    private:

      std::string bisector[2];
      bool vertex[2];

    public:

      edge();
      edge(const node* n1, const node* n2);
      ~edge();

      const std::string* get_sites() const;
      void tie_first_point(const point2d point);
      void tie_second_point(const point2d point);
      bool is_p1_defined();
      bool is_p2_defined();

    };

    void node_to_point(const node * node, point2d* point);
  }
}

#endif
