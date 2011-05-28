#include <set>
#include <list>

#include "voronoi.hpp"
#include "point2d.hpp"


namespace librecube {
  namespace network {

    /**
     * @brief Creates a voronoi diagram
     */
    voronoi::voronoi():
      width(0),
      height(0),
      edges(){

    }

    /**
     * @brief Destructor.
     */
    voronoi::~voronoi() {
     
    }

    const double voronoi::get_width() const {
      
      return this->width;
    }

    const double voronoi::get_height() const {
      
      return this->height;
    }

    bool point_x_is_lesser_than(const point2d p1, const point2d p2) {

      bool res = false;

      if (p1.get_x() < p2.get_x()) {
	res = true;
      } else if (p1.get_x() == p2.get_x()) {
	res = (p1.get_y() < p2.get_y());
      } 
      return res;
    }
    
    void find_breakpoint(const node& s1, const node& s2, const node& s3, point2d& res) {

      edge b1, b2;
      
      b1 = edge(&s1,&s2);
      b2 = edge(&s1,&s3);
      b1.intersection(b2, res);
      std::cout << "inter: " << res.get_x() << "," << res.get_y() << std::endl; 
     }

    void get_lowest_point_of_circle(const node& s1, const node& s2, const node& s3, point2d& res) {
      
      point2d a, b, c;
      double r;
      
      node_to_point(&s1,&a);
      node_to_point(&s2,&b);
      r = a.distance(b);
      find_breakpoint(s1, s2, s3, c);
      res.set_x(c.get_x());
      res.set_y(c.get_y() + r);
    }

    bool circle_is_lesser_than(const std::vector<node> e1, const std::vector<node> e2) {

      point2d p1, p2;
 
      get_lowest_point_of_circle(e1[0], e1[1], e1[2], p1);
      get_lowest_point_of_circle(e2[0], e2[1], e2[2], p2);
      return point_x_is_lesser_than(p1, p2);
    }


    struct sitexlesserthan {

      // Compare function to go left->right
      bool operator()(const node n1, const node n2) const {
	
	point2d p1, p2;
	
	node_to_point(&n1,&p1);
	node_to_point(&n2,&p2);
	return point_x_is_lesser_than(p1, p2);
      }
    };

    struct circlelesserthan {

      // Compare function to go left->right
      bool operator()(const std::vector<node> e1, const std::vector<node> e2) const {

	return circle_is_lesser_than(e1, e2);
      }
    };

    void voronoi::set_first_vertice_point(const std::string& i, const std::string& j, const point2d& point) {

      std::vector<edge> edges_i, edges_j;
      std::vector<edge>::iterator it;

      edges_i = this->edges[i];
      edges_j = this->edges[j];

      for (it=edges_i.begin(); it!= edges_i.end(); it++) {
	const std::string * ids = it->get_sites();
	if (ids[0] == i && ids[1] ==  j) {
	  it->tie_first_point(point);
	  break;
	}
      }
      for (it=edges_j.begin(); it!= edges_j.end(); it++) {
	const std::string * ids = it->get_sites();
	if (ids[0] == i && ids[1] ==  j) {
	  it->tie_first_point(point);
	  break;
	}
      }
      this->edges[i] = edges_i;
      this->edges[j] = edges_j;      
     }

    void voronoi::set_second_vertice_point(const std::string& i, const std::string& j, const point2d& point) {

      std::vector<edge> edges_i, edges_j;
      std::vector<edge>::iterator it;

      edges_i = this->edges[i];
      edges_j = this->edges[j];

      for (it=edges_i.begin(); it!= edges_i.end(); it++) {
	const std::string * ids = it->get_sites();
	if (ids[0] == i && ids[1] ==  j) {
	  it->tie_second_point(point);
	  break;
	}
      }
      for (it=edges_j.begin(); it!= edges_j.end(); it++) {
	const std::string * ids = it->get_sites();
	if (ids[0] == i && ids[1] ==  j) {
	  it->tie_second_point(point);
	  break;
	}
      }
      this->edges[i] = edges_i;
      this->edges[j] = edges_j;      
    }
  
    void voronoi::generate_diagram(std::map<std::string, node> nodes) {
      
      double min_x, min_y, max_x, max_y;
      std::map<std::string, node>::const_iterator it;
      std::set<node, sitexlesserthan> site_queue;
      std::set<std::vector<node>, circlelesserthan > vertex_queue;
      std::list<node> beachline;
      std::list<node>::iterator b_it;
      
      node current_site;
      std::vector<node> current_vertex;
      bool site_event;

      // Clean old voronoi
      this->edges.clear();

      // Compute the node queue and plane coordinates 
      for (it=nodes.begin(); it!=nodes.end(); it++) {
	point2d point;
	double x, y;
	node n;
	
	n = it->second;
	site_queue.insert(n);

	x = point.get_x();
	y = point.get_y();
	if (x < min_x)
	  min_x = x;
	if (x > max_x)
	  max_x = x;
	if (y < min_y)
	  min_y = y;
	if (y > max_y)
	  max_y = y;
      }
      
      // for (b_it=beachline.begin(); b_it!=beachline.end(); b_it++) {
      
      // 	if (&*b_it !=  &beachline.front() && &*b_it != &beachline.back()) {
      // 	  std::list<point2d>::iterator elt;
 	  
      // 	  elt = b_it;
      // 	  std::cout << "triplet :";
      // 	  elt--;
      // 	  std::cout << "(" << elt->get_x() << "," << elt->get_y() << ")";
      // 	  elt++;
      // 	  std::cout << "(" << elt->get_x() << "," << elt->get_y() << ")";
      // 	  elt++;
      // 	  std::cout << "(" << elt->get_x() << "," << elt->get_y() << ")";
      // 	  std::cout << std::endl;
      // 	}
      // }
      // std::cout << site_queue.size() << " sorted points: ";
      // while (!site_queue.empty())
      // 	  {
      // 	    std::cout << "(" << site_queue.top().get_x() << "," << site_queue.top().get_y() << ")";
      // 	    site_queue.pop();
      // 	  }
      // std::cout << std::endl;




      // Sweep line algorithm, horizontal, from left to right
      
      while (!site_queue.empty() || !vertex_queue.empty()) {
	
	// select event type
	if (site_queue.empty()) {
	  site_event = false;
	} 
	else if (vertex_queue.empty()) {
	  site_event = true;
	} 
	else {
	  point2d p1, p2;
	  std::vector<node> e;
	  double y;
	  
	
	  node_to_point(&*site_queue.begin(),&p1);
	  e = *vertex_queue.begin();
	  get_lowest_point_of_circle(e[0], e[1], e[2], p2);
	  site_event = point_x_is_lesser_than(p1, p2);
	}
	
	  std::cout << "beachline size " << beachline.size() << std::endl;

	  for (b_it=beachline.begin(); b_it!=beachline.end(); b_it++) {
	    std::cout << b_it->get_id() << ": " << b_it->get_position().get_z() << "," << b_it->get_position().get_x() << std::endl;
 	  } 

	
	// next event is vertex event
	if (!site_event) {

	  current_vertex = *vertex_queue.begin();
	  std::cout << "vertex size " << vertex_queue.size() << std::endl;

	  vertex_queue.erase(vertex_queue.begin());
	  
	  std::cout << "vertex! " << current_vertex[0].get_position().get_z() << "," << current_vertex[0].get_position().get_x() << " " << current_vertex[1].get_position().get_z() << "," << current_vertex[1].get_position().get_x() << " " << current_vertex[2].get_position().get_z() << "," << current_vertex[2].get_position().get_x() << std::endl;
	  
	  point2d breakpoint;
	  node i, j, k, a, b, c, m, n;
	  edge new_edge;
	  std::list<node>::iterator b_it_next;   
	    
	  i = current_vertex[0];
	  j = current_vertex[1];
	  k = current_vertex[2]; 
	  find_breakpoint(i, j, k, breakpoint);
	  
	  set_second_vertice_point(i.get_id() ,j.get_id() ,breakpoint);
	  set_second_vertice_point(j.get_id() ,k.get_id() ,breakpoint);
	  new_edge = edge(&i, &k);
	  this->edges[i.get_id()].push_back(new_edge);
	  this->edges[k.get_id()].push_back(new_edge);
	  set_first_vertice_point(i.get_id() ,k.get_id() ,breakpoint);
	  print_edges();

	  std::cout << "triplet" << i << " " << j << " " << k << std::endl;
	  
	  bool m_ok, n_ok = false;
	  // remove j from beachline
	  for (b_it=beachline.begin(); b_it!=beachline.end(); b_it++) {
	    std::list<node>::iterator cursor;
	    cursor = b_it;
	    if (cursor != beachline.begin()) {
	      cursor--;
	      if (cursor != beachline.begin()) {
		m_ok = true;
		std::cout << "m " << m << std::endl;
		std::cout << "c " << *cursor << std::endl;
		m = *cursor;
	      }
	      cursor++;
	    }
	    std::cout << *cursor << std::endl;
	    a = *cursor;
	    if (a == i) {
	      cursor++;
	      // not needed but for the fun
	      if (cursor != beachline.end()) {
		b = *cursor;
		if (b == j) {
		  cursor++;
		  if (cursor != beachline.end()) {
		    c = *cursor;
		    if (c == k) {
		      cursor++;
		      if (cursor != beachline.end()) {
			n_ok = true;
			n = *cursor;
		      }
		      if (m_ok) {
			std::vector<node> t(3), erase(3);
			erase[0] = m; erase[1] = a; erase[2] = b;
			vertex_queue.erase(erase);
			t[0] = m; t[1] = a; t[2] = c;
			vertex_queue.insert(t);
		      }
		      if (n_ok) {
			std::vector<node> t(3), erase(3);
			erase[0] = b; erase[1] = c; erase[2] = n;
			vertex_queue.erase(erase);
			t[0] = a; t[1] = c; t[2] = n;
			vertex_queue.insert(t);
		      }
		      cursor--;
		      cursor--;
		      beachline.erase(cursor);
		      
		    }
		  } 
		  else {
		    std::cout << "should not happen " << std::endl;
		    break;
		  }
		}
	      }
	      else {
		std::cout << "should not happen " << std::endl;
		break;
	      }
	    }
	  }
	} 
	
	// next event is site event
	else {
	  current_site = *site_queue.begin();
	  site_queue.erase(site_queue.begin());

	  std::cout << "site" << std::endl;

	  // find site whose arc on the beachline collide with the parabol of the site on the sweepline
	  // to acheive this, we compute each breakpoints of edges on the beachline, and figure out between which pair the parabol of the sweeplin site collide.
	  
	  if (beachline.size() == 0) {
	    beachline.push_back(current_site);
	  }
	  else {
	    edge new_edge;
	    std::list<node>::iterator b_it_next;   
	
	    for (b_it=beachline.begin(); b_it!=beachline.end(); b_it++) {
	      std::cout << "current: " << b_it->get_id() << ": " << b_it->get_position().get_z() << "," << b_it->get_position().get_x() << std::endl;
	      
	      b_it_next = b_it;
	      b_it_next++;
	      if (b_it_next == beachline.end()) {
		node j;
		std::vector<node> triplet (3);  
		std::cout << "ici " << b_it->get_id() << std::endl;
		
		j = *b_it;
		triplet[0] = j;
		triplet[1] = current_site; 
		triplet[2] = j;
		beachline.push_back(current_site);
		beachline.push_back(j);
		new_edge = edge(&current_site, &j);
		this->edges[current_site.get_id()].push_back(new_edge);
		this->edges[j.get_id()].push_back(new_edge);
		print_edges();
		break;
	      }
	      else {
		point2d breakpoint;
		std::cout  << "la" << std::endl;
		node i, j, k;
		j = *b_it;
		k = *b_it_next;
		std::cout << "j " << j.get_position().get_z() << "," << j.get_position().get_x() << std::endl;;
		std::cout << "k " << k.get_position().get_z() << "," << k.get_position().get_x() << std::endl;;

		find_breakpoint(j, k, current_site, breakpoint);
		std::cout << breakpoint.get_x() << "," << breakpoint.get_y() << std::endl;;
		std::cout << current_site.get_position().get_z() << "," << current_site.get_position().get_x() << std::endl;;
		std::cout << breakpoint.get_y() << ">" << current_site.get_position().get_x() << std::endl;;
		if (breakpoint.get_y() > current_site.get_position().get_x()) {
		  std::cout << ">" << std::endl;
		  std::vector<node> t1 (3), t2 (3), erase (3);
		  		  
		  if (b_it != beachline.begin()) {
		    b_it--;
		    i = *b_it;
		    std::cout << "insert 1" << std::endl;
		    if (!(i == current_site)) {
		      t1[0] = i; t1[1] = j; t1[2] = current_site;
		      vertex_queue.insert(t1);
		      std::cout << "size1 " << vertex_queue.size() << std::endl;
		      erase[0] = i; erase[1] = j; erase[2] = k;
		      vertex_queue.erase(erase);
		      std::cout << "size2 " << vertex_queue.size() << std::endl;
		      b_it++;
		    }
		  }
		  if (!(current_site == k)) {
		    std::cout << "insert 2" << std::endl;
		    t2[0] = current_site; t2[1] = j; t2[2] = k;
		    vertex_queue.insert(t2);
		      std::cout << "size3 " << vertex_queue.size() << std::endl;
		  }
		  beachline.insert(b_it_next, current_site);
 		  beachline.insert(b_it_next, j);
		  new_edge = edge(&current_site, &j);
		  this->edges[current_site.get_id()].push_back(new_edge);
		  this->edges[j.get_id()].push_back(new_edge);
		  print_edges();
		  break;
		}
	      }
	    }
	  }
	}
      }
      print_edges();
    }
    
    void voronoi::print_edges() {
      std::map<std::string, std::vector<edge> >::const_iterator it3;
 
      std::cout << "edges :" << std::endl;
      for (it3=this->edges.begin(); it3!=this->edges.end(); it3++){
	
	std::cout << it3->first << std::endl;
	std::vector<edge>::iterator it2;
	std::vector<edge> es = it3->second;

	for (it2=es.begin(); it2!=es.end(); it2++) {
	  const std::string * ids = (*it2).get_sites();
	  std::cout << "(" << ids[0] << "," << ids[1] << ")" << std::endl;
	  if ((*it2).is_p1_defined())
	    std::cout << (*it2).get_p1().get_x() << "," << (*it2).get_p1().get_y() << " ";
	  else
	    std::cout << "undefined1 ";
	  if ((*it2).is_p2_defined())
	    std::cout << (*it2).get_p2().get_x() << "," << (*it2).get_p2().get_y() << " ";
	  else
	    std::cout << "undefined2 ";
	  std::cout << std::endl;
	}
      }
    }
  }
}
