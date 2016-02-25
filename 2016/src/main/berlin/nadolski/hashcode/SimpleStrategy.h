#ifndef SIMPLESTRATEGY_H_
#define SIMPLESTRATEGY_H_

#include "ProblemState.h"
#include "Strategy.h"

namespace berlin {
namespace nadolski {
namespace hashcode {

class SimpleStrategy: public ProblemState, Strategy {
public:
   SimpleStrategy(std::istream &in): ProblemState(in) {}

   /*
    * return an "optimized" solution
    */
   std::list<Command> generate_commands();

private:
   /*
    * get the first best warehouse which contains a certain product.
    * order is given by how it is stored.
    */
   int get_warehouse_from_product(int);
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* SIMPLESTRATEGY_H_ */
