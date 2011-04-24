#include "world.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>

namespace librecube {
  namespace graphics {

    world::world() {
      cam.set_position(vector(-20, 10, 2));
      ava.set_position(cam.get_position());
      ind.set_map_position(cam.get_position());
      blocs = librecube::generate_world();
    }

    world::~world() {

    }

    void world::process_event(sf::Event& event) {

      if (event.Type == sf::Event::KeyPressed) {

        switch (event.Key.Code) {

          case sf::Key::Z:
            cam.move_forward(1);
	    ava.set_position(cam.get_position());
	    ind.set_map_position(cam.get_position());
            break;

          case sf::Key::S:
            cam.move_backward(1);
	    ava.set_position(cam.get_position());
	    ind.set_map_position(cam.get_position());
            break;

          case sf::Key::D:
            cam.move_right(1);
	    ava.set_position(cam.get_position());
	    ind.set_map_position(cam.get_position());
            break;

          case sf::Key::Q:
            cam.move_left(1);
	    ava.set_position(cam.get_position());
	    ind.set_map_position(cam.get_position());
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
            ind.send_helloworld();
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
      ind.tick();
      ind.receive_msg();
    }

    void world::draw() {

      // look towards the camera's direction
      const vector& position = cam.get_position();
      const vector& target = cam.get_target();
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
 
      // display others avatars
      const std::vector<Vast::Node *> neighbors = ind.get_neighbors();
      size_t j;
      std::map<Vast::id_t, avatar>::iterator it;

      for(j=0; j< neighbors.size(); j++) {
	Vast::id_t id = neighbors[j]->id;
	double x = (double) neighbors[j]->aoi.center.x;
	double y = (double) neighbors[j]->aoi.center.z;
	double z = (double) neighbors[j]->aoi.center.y;
	it = oth_avatars.find(id);
	if (it == oth_avatars.end()) {
	  it = oth_avatars.begin();
	  avatar av;	  
	  av.set_position(vector(x,y,z));
	  it = oth_avatars.insert(it, std::make_pair(id,av)); 
	} else {
	  it->second.set_position(vector(x,y,z));
	}
	test_block.draw(it->second.get_position());
      }
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
      std::cout << endl;
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
