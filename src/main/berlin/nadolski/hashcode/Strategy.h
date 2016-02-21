#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "Problem.h"
#include "Command.h"

#include <list>
#include <string>

namespace berlin {
namespace nadolski {
namespace hashcode {

bool validate(const std::list<Command> &commands, const Problem &problem);
int simulate(const std::list<Command> &commands, const Problem &problem);

class Strategy {
public:
   Strategy(const Problem &p);
   virtual ~Strategy();

   virtual std::list<Command> generate_commands() = 0;
   virtual std::string name() = 0;

   Problem problem;
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* STRATEGY_H_ */
