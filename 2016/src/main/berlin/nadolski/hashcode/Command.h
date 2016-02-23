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
   Command(int drone, Type t, int dest, int product, int value);
   virtual ~Command();

   Type type;
   int drone;
   int order;
   int warehouse;
   int product;
   int value;
 
   bool operator==(const Command &command) {
      return this == &command;
   }
};

std::ostream &operator<<(std::ostream &out, const Command &cmd);

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* COMMAND_H_ */
