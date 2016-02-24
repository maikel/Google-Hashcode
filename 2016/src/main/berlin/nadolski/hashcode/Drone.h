#ifndef DRONE_H_
#define DRONE_H_

#include <iostream>
#include "Command.h"

namespace berlin {
namespace nadolski {
namespace hashcode {

struct Drone {
   int busy;
   Command command;
   int x;
   int y;
   int load;
   int id;

   bool operator==(const Drone &d) const
   {
      return id == d.id;
   }
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* DRONE_H_ */
