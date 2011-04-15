#ifndef LIBRECUBE_CUBE_HPP
#define LIBRECUBE_CUBE_HPP

#include <istream>
#include <ostream>
#include <string>

namespace librecube {
   namespace world {

      /**
       * @brief Represents a cube in the world.
       */
      class cube {
         public:
            typedef unsigned char type_t;
            static const type_t undef = 0;
         public:
            type_t      type;
            std::string data;
         public:
            cube(void);
            cube(type_t type, const std::string& data);
            friend std::ostream& operator <<(std::ostream& stream, const cube& obj);
            friend std::istream& operator >>(std::istream& stream, cube& obj);
      };
   }
}

#endif
