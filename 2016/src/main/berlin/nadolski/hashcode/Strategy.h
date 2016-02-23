#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "Problem.h"
#include "Command.h"

#include <list>
#include <string>

namespace berlin {
namespace nadolski {
namespace hashcode {

class Strategy {
public:
   virtual ~Strategy() { }
   virtual std::list<Command> generate_commands() = 0;
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* STRATEGY_H_ */
