#include "cube.hpp"

namespace librecube {
   namespace world {
      cube::cube(void)
           :type(undef), data() {
      }

      cube::cube(type_t type, const std::string& data)
           :type(type), data(data) {
      }

      std::ostream& operator <<(std::ostream& stream, const cube& obj) {
         size_t      size  = obj.data.length();
         const char* bytes = obj.data.data(); 

         stream << obj.type << size;
         for(size_t i = 0; i < size; i++) {
            stream << bytes[i];
         }
         return stream;
      }

      std::istream& operator >>(std::istream& stream, cube& obj) {
         size_t size;

         stream >> obj.type >> size;
         obj.data.clear();
         obj.data.reserve(size);
         for(size_t i = 0; i < size; i++) {
            char c;

            stream.get(c);
            obj.data.push_back(c);
         }
         return stream;
      }
   }
}
