#include "Drone.h"

namespace berlin {
namespace nadolski {
namespace hashcode {

Drone::Drone(): Drone(-1,-1) {}

Drone::Drone(int _x, int _y): busy(0), command(), x(_x), y(_y), load(0) {
   // TODO Auto-generated constructor stub

}

Drone::~Drone() {
   // TODO Auto-generated destructor stub
}


} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */
