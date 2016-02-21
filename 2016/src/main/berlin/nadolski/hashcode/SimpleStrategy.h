#ifndef SIMPLESTRATEGY_H_
#define SIMPLESTRATEGY_H_

#include "Strategy.h"

namespace berlin {
namespace nadolski {
namespace hashcode {

class SimpleStrategy: public Strategy {
public:
   SimpleStrategy(const Problem &p): Strategy(p) {}

   std::list<Command> generate_commands();
   std::string name() { return std::string("SimpleStrategy"); }
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* SIMPLESTRATEGY_H_ */
