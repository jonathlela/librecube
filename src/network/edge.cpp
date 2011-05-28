#include "edge.hpp"
#include <iostream>

namespace librecube {
  namespace network {

    edge::edge() {
      
      this->vertex[0] = false;
      this->vertex[1] = false;
    }

    edge::edge(const node* n1, const node* n2) {
      
      point2d p, q;
      line2d l;
      double x, y;

      this->vertex[0] = false;
      this->vertex[1] = false;

      this->bisector[0] = n1->get_id();
      this->bisector[1] = n2->get_id();

      //compute line passing by n1 and n2
      node_to_point(n1, &p);
      node_to_point(n2, &q);
      l = line2d(p,q);

      x = (p.get_x() + q.get_x()) / 2;
      y = (p.get_y() + q.get_y()) / 2;
      
      this->a = -l.get_b();
      this->b = l.get_a();
      this->c = this->a * x + this->b * y;

      // dx = x2 - x1 ;
      // dy = y2 - y1 ;
      // adx = dx>0 ? dx : -dx ;
      // ady = dy>0 ? dy : -dy ;
      // this->c = x1 * dx + y1 * dy + (dx * dx + dy * dy) * 0.5 ;
      // if (adx > ady) {
      // 	this->a = 1.0 ;
      // 	this->b = dy/dx ;
      // 	this->c /= dx ;
      // }
      // else {
      // 	this->b = 1.0 ;
      // 	this->a = dx/dy ;
      // 	this->c /= dy ;
      // }
  
    }

    edge::~edge() {

    }

    const std::string* edge::get_sites() const {

      return this->bisector;
    }

    void edge::tie_first_point(const point2d point) {

      this->vertex[0] = true;
      this->p1 = point;
    }

    void edge::tie_second_point(const point2d point) {

      this->vertex[1] = true;     
      this->p2 = point;
    }

    bool edge::is_p1_defined(){

      return this->vertex[0];
    }    

    bool edge::is_p2_defined(){

      return this->vertex[1];
    }    
   
    
    void node_to_point(const node* node, point2d* point) {
      
      point->set(node->get_position().get_z(), node->get_position().get_x());
    }

  }
}
