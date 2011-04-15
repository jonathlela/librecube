#ifndef LIBRECUBE_BLOCK_HPP
#define LIBRECUBE_BLOCK_HPP

#include "vector.hpp"

namespace librecube {
  namespace graphics {

    /**
     * @brief Represents a block placed in the world.
     *
     * TODO: implement several types of block
     */
    class block {

      public:

        block();
        ~block();

        void draw(const vector& position);
    };

  }
}

#endif
