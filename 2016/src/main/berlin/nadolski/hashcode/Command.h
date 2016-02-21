#ifndef COMMAND_H_
#define COMMAND_H_

#include <ostream>

namespace berlin {
namespace nadolski {
namespace hashcode {

class Command {
public:
   Command();
   virtual ~Command();

   enum Type {
      LOAD    = 'L',
      UNLOAD  = 'U',
      WAIT    = 'W',
      DELIVER = 'D',
      INVALID = 'I'
   };

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