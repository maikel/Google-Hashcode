#ifndef COMMAND_H_
#define COMMAND_H_

#include <ostream>

namespace berlin {
namespace nadolski {
namespace hashcode {

class Command {
public:
   enum Type {
         load    = 'L',
         unload  = 'U',
         wait    = 'W',
         deliver = 'D',
         invalid = 'I'
   };

   Command();
   Command(std::size_t drone, Type t, std::size_t dest, std::size_t product, std::size_t value);
   virtual ~Command();

   Type type;
   std::size_t drone;
   std::size_t order;
   std::size_t warehouse;
   std::size_t product;
   std::size_t value;
};

std::ostream &operator<<(std::ostream &out, const Command &cmd);

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* COMMAND_H_ */
