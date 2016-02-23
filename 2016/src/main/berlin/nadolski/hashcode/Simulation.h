#ifndef BERLIN_NADOLSKI_HASHCODE_SIMULATION_H_
#define BERLIN_NADOLSKI_HASHCODE_SIMULATION_H_

#include <istream>
#include <list>
#include "Command.h"
#include "Problem.h"

namespace berlin {
namespace nadolski {
namespace hashcode {

class Simulation {
public:
   Simulation(std::istream &in);
   virtual int run(std::list<Command> commands);
   virtual ~Simulation();
   Problem problem;
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* BERLIN_NADOLSKI_HASHCODE_SIMULATION_H_ */
