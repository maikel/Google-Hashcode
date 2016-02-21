#ifndef DRONE_H_
#define DRONE_H_

#include "Command.h"

namespace berlin {
namespace nadolski {
namespace hashcode {

class Drone {
public:
   Drone();
   Drone(int, int);
   virtual ~Drone();

   int busy;
   Command command;
   int x;
   int y;
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* DRONE_H_ */
