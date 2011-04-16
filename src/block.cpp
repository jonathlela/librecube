#include "block.hpp"
#include <GL/gl.h>

namespace librecube {
  namespace graphics {

    /**
     * @brief Constructor.
     */
    block::block() {

    }

    /**
     * @brief Destructor.
     */
    block::~block() {

    }

    /**
     * @brief Draws this block at the specified position.
     * @param position coordinates of where the center of this block should be displayed
     */
    void block::draw(const vector& position) {

      glPushMatrix();
      glTranslated(position.get_x(), position.get_y(), position.get_z());
      glBegin(GL_QUADS);

      glColor3ub(192, 96, 0);
      glVertex3f( 0.5f, 0.5f, 0.5f);
      glVertex3f(-0.5f, 0.5f, 0.5f);
      glVertex3f(-0.5f,-0.5f, 0.5f);
      glVertex3f( 0.5f,-0.5f, 0.5f);

      glColor3ub(192, 96, 0);
      glVertex3f( 0.5f, 0.5f,-0.5f);
      glVertex3f(-0.5f, 0.5f,-0.5f);
      glVertex3f(-0.5f,-0.5f,-0.5f);
      glVertex3f( 0.5f,-0.5f,-0.5f);

      glColor3ub(192, 96, 0);
      glVertex3f( 0.5f, 0.5f, 0.5f);
      glVertex3f( 0.5f,-0.5f, 0.5f);
      glVertex3f( 0.5f,-0.5f,-0.5f);
      glVertex3f( 0.5f, 0.5f,-0.5f);

      glColor3ub(192, 96, 0);
      glVertex3f(-0.5f, 0.5f, 0.5f);
      glVertex3f(-0.5f,-0.5f, 0.5f);
      glVertex3f(-0.5f,-0.5f,-0.5f);
      glVertex3f(-0.5f, 0.5f,-0.5f);

      glColor3ub(0, 192, 0);
      glVertex3f( 0.5f, 0.5f, 0.5f);
      glVertex3f( 0.5f, 0.5f,-0.5f);
      glVertex3f(-0.5f, 0.5f,-0.5f);
      glVertex3f(-0.5f, 0.5f, 0.5f);

      glColor3ub(192, 96, 0);
      glVertex3f( 0.5f,-0.5f, 0.5f);
      glVertex3f( 0.5f,-0.5f,-0.5f);
      glVertex3f(-0.5f,-0.5f,-0.5f);
      glVertex3f(-0.5f,-0.5f, 0.5f);

      glEnd();
      glPopMatrix();
    }

  }
}
