#ifndef DRONE_H_
#define DRONE_H_

#include <iostream>
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
   int load;

   bool operator==(const Drone &d) const
   {
      return this == std::addressof(d);
   }
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* DRONE_H_ */
