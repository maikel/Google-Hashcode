#ifndef SIMPLESTRATEGY_H_
#define SIMPLESTRATEGY_H_

#include "Strategy.h"

namespace berlin {
namespace nadolski {
namespace hashcode {

class SimpleStrategy: public Problem, Strategy {
public:
   SimpleStrategy(std::istream &in): Problem(in) {}

   /*
    * return an "optimized" solution
    */
   std::list<Command> generate_commands();

   /*
    * print global parameters defined by the input file
    */
   void print_parameter(std::ostream &out);

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
