#ifndef LIBRECUBE_VORONOI_HPP
#define LIBRECUBE_VORONOI_HPP

#include <map>
#include <vector>
#include <string>
#include "point2d.hpp"
#include "edge.hpp"
#include "node.hpp"

namespace librecube {
  namespace network {

    /**
     * @brief A Voroni diagram
     */
    class voronoi {

      private:

        double width, height;
        std::map<std::string, std::vector<edge> > edges;

        void set_first_vertice_point(const std::string& i, const std::string& j, const point2d& point);
        void set_second_vertice_point(const std::string& i, const std::string& j, const point2d& point);

      void print_edges();

      public:

        voronoi();
        ~voronoi();
      
        void generate_diagram(const std::map<std::string, node> nodes);

        const double get_width() const;
        const double get_height() const;

    };

  }
}

#endif
