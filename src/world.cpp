#include "world.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <GL/gl.h>
#include <SFML/Network.hpp>

namespace librecube {
  namespace graphics {

    const double PI = 3.14159265358979323846; // PI from <cmath> is not portable

    world::world(): peer() {

      sf::IPAddress address(sf::IPAddress::GetLocalAddress());
      unsigned short port = 5436;

      cam.set_position(vector(-20, 10, 2));
      top.set_position(vector(0, 50, 0));
      top.look_down(90);
      ava.set_position(cam.get_position());
      blocs = librecube::generate_world();
      current_cam = &cam;
      peer.join_gateway(address, port);
      peer.set_position(cam.get_position());
    }

    world::~world() {

    }

    void world::process_event(sf::Event& event) {

      if (event.Type == sf::Event::KeyPressed) {

        switch (event.Key.Code) {

          case sf::Key::Z:
            cam.move_forward(1);
	    ava.set_position(cam.get_position());
	    peer.set_position(cam.get_position());
            break;

          case sf::Key::S:
            cam.move_backward(1);
	    ava.set_position(cam.get_position());
	    peer.set_position(cam.get_position());
            break;

          case sf::Key::D:
            cam.move_right(1);
	    ava.set_position(cam.get_position());
	    peer.set_position(cam.get_position());
            break;

          case sf::Key::F:
	    if (current_cam == &cam) {
	      current_cam = &top;
	    } else {
	      current_cam = &cam;
	    }     
            break;


          case sf::Key::Q:
            cam.move_left(1);
	    ava.set_position(cam.get_position());
	    peer.set_position(cam.get_position());
            break;

          case sf::Key::Right:
            cam.look_right(2);
            break;

          case sf::Key::Left:
            cam.look_left(2);
            break;

          case sf::Key::Up:
            cam.look_up(2);
            break;

          case sf::Key::Down:
            cam.look_down(2);
            break;

          case sf::Key::H:
            peer.send_msg(std::string("Hello world !"));
            break;

          default:
            break;
        }
      }
      else if (event.Type == sf::Event::MouseMoved) {
        // TODO
      }
    }

    void world::update() {
      //ind.tick();
      //ind.receive_msg();
    }

    void world::draw_minimap() { 
  
      const std::map<std::string, network::node> neighbors = peer.get_voronoi_nodes();
      std::map<std::string, network::node>::const_iterator it;

      // ortho
      glMatrixMode( GL_PROJECTION );
      glPushMatrix();
      glLoadIdentity();
      glOrtho( 0.0f, 800.0f, 600.0f, 0.0f, -100.0f, 100.0f );
      // topright
      glMatrixMode( GL_MODELVIEW );
      glPushMatrix();
      glLoadIdentity();
      glTranslatef( 671.0f, 0.0f, 0.0f );
      // display peers
      glPushMatrix();
      glTranslatef( 64.0f, 64.0f, 0.0f );
      float scale = 129.0f / (2*network::node::RADIUS + 1);
      glScaled(scale, scale,scale);
      // display other peers
      for (it=neighbors.begin(); it!=neighbors.end(); it++) {
	vector minimap_pos = it->second.get_position() - peer.get_position();
	double x, y;
	x = minimap_pos.get_z();
	y = - minimap_pos.get_x();
	glPushMatrix();
	glTranslated( x, y, 0.0d );
	glColor4f( 0.0f, 1.0f, 0.0f, 1.0f );
	glBegin( GL_QUADS );
	glVertex2f( 0.0f, 1.0f );
	glVertex2f( 1.0f, 1.0f );
	glVertex2f( 1.0f, 0.0f );
	glVertex2f( 0.0f, 0.0f );
	glEnd();
	glPopMatrix();
      }
      //display self
      glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
      glBegin( GL_QUADS );
      glVertex2f( 0.0f, 1.0f );
      glVertex2f( 1.0f, 1.0f );
      glVertex2f( 1.0f, 0.0f );
      glVertex2f( 0.0f, 0.0f );
      glEnd();
      glPopMatrix();
      // display bg
      glColor4f( 1.0f, 1.0f, 1.0f, 0.3f );
      glBegin( GL_QUADS );
      glVertex2f( 0.0f, 129.0f );
      glVertex2f( 129.0f, 129.0f );
      glVertex2f( 129.0f, 0.0f );
      glVertex2f( 0.0f, 0.0f );
      glEnd();
      glPopMatrix();
      glPushMatrix();
      glLoadIdentity();
      glPopMatrix();
      glMatrixMode( GL_PROJECTION );
      glPopMatrix();
    }
    

    void world::draw() {

      // look towards the camera's direction
      const vector& position = current_cam->get_position();
      const vector& target = current_cam->get_target();
      gluLookAt(
          position.get_x(), position.get_y(), position.get_z(),  // current position
          target.get_x(), target.get_y(), target.get_z(),        // where to look
          0, 1, 0                                                // vertical axis
          );

      // display a block
      vector where;
      int x, y, z, w_x, w_y, w_z;
      w_x = w_y = w_z = 32;
      for (x=0; x<w_x; x++) {
      	where.set_z(x);
      	for (y=0; y<w_y; y++) {
      	  where.set_x(y);
          for (z=0; z<w_z; z++) {
            if (blocs[x*w_y*w_z+y*w_z+z]) {
      	      where.set_y(z);
	      
              test_block.draw(where);
            }
      	  }
      	}
      }
      /*for (int i = -8; i < 8; i+=2) {
        where.set_x(i);
        for (int j = -8; j < 8; j+=2) {
          where.set_z(j);
          for (int k = -8; k < 8; k+=2) {
            where.set_y(k);
            test_block.draw(where);
          }
        }
      }*/

      //display self
      test_block.draw(peer.get_position());

      // display others avatars
      const std::map<std::string, network::node> neighbors = peer.get_aoi_neighbors();
      std::map<std::string, network::node>::const_iterator itn;
      std::map<std::string, avatar>::iterator ita;

      for (itn=neighbors.begin(); itn!=neighbors.end(); itn++) {
	std::string id = itn->first;
	ita = oth_avatars.find(id);
	if (ita == oth_avatars.end()) {
	  ita = oth_avatars.begin();
	  avatar av;	  
	  av.set_position(itn->second.get_position());
	  ita = oth_avatars.insert(ita, std::make_pair(id,av)); 
	} else {
	  ita->second.set_position(itn->second.get_position());
	}
	test_block.draw(itn->second.get_position());
      }

      // int k; 
      //
      //display voronoi
      // if (current_cam == &top) {
      // 	std::vector<Vast::line2d> &lines = ind.get_voronoi()->getedges();
      // 	std::cout << "voronoi :" << ind.get_voronoi()->size() << " " << lines.size() << std::endl;
      //   for (k=0; k < lines.size (); k++)
      //   { 
      //     std::cout << "lines :" << lines[k].seg.p1.x << lines[k].seg.p1.y << lines[k].seg.p2.x << lines[k].seg.p2.y << std::endl;
      // 	  glBegin(GL_LINES);
      // 	  glVertex3d(lines[k].seg.p1.x, 30, lines[k].seg.p1.y);
      // 	  glVertex3d(lines[k].seg.p2.x, 30, lines[k].seg.p1.y);
      // 	  glEnd( );
      //   }               
      // }

      //display minimap;
      draw_minimap();
    }
  }

  int* generate_world(){
    int x, y, z;
    int w_x, w_y, w_z;
    w_x = w_y = w_z = 32;

    // initialize random seed
    srand ( time(NULL) );

    int kp_ratio = 4;
    // create keypoints
    int kp_sx = w_x/kp_ratio+1;
    int kp_sy = w_y/kp_ratio+1;
    int keypoints[kp_sx*kp_sy];
    for (x=0; x<kp_sx; x++) {
      for (y=0; y<kp_sy; y++) {
        keypoints[x*kp_sy+y] = floor(rand() % 20)+2;
	std::cout << keypoints[x*kp_sy+y] << " ";
      }
      std::cout << std::endl;
    }

    // create world
    int* board = (int*) malloc(sizeof(int)*w_x*w_y*w_z);
    int mh, kfx, kfy, sx, sy;
    float kx, ky, p, v, pvs, ps;
    for (x=0; x<w_x; x++) {
      for (y=0; y<w_y; y++) {
        mh = 1;
	kx = x/float(kp_ratio); kfx = floor(kx);
	ky = y/float(kp_ratio); kfy = floor(ky); 
	if (kx==kfx && ky==kfy) {
	  mh = keypoints[kfx*kp_sy+kfy];
	}else{
	  // bilinear interpolation
	  pvs = ps = 0;
	  for (sx=0; sx<=1; sx++) {
	    for (sy=0; sy<=1; sy++) {
	      p = 1/sqrt(pow(kx-(kfx+sx),2)+pow(ky-(kfy+sy), 2));
	      v = keypoints[(kfx+sx)*kp_sy+(kfy+sy)];
	      pvs = pvs + p*v;
	      ps = ps + p;
            }
	  }
	  mh = floor(pvs/ps);
	}
	//std::cout << mh << " ";
        for (z=0;z<mh;z++) {
	    board[x*w_y*w_z+y*w_z+z] = 1;
	}
	for (;z<w_z;z++) {
	    board[x*w_y*w_z+y*w_z+z] = 0;
	}
      }
      //std::cout << endl;
    }
    return board;
  } 

}
