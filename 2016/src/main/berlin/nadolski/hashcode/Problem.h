#ifndef PROBLEM_H_
#define PROBLEM_H_

#include "Order.h"
#include "Drone.h"
#include "Warehouse.h"

#include <iostream>
#include <vector>

namespace berlin {
namespace nadolski {
namespace hashcode {

struct Problem {
   Problem(std::istream &input);
   virtual ~Problem();

   // print parameters for this simulation
   void print_parameter(std::ostream &out);

   // given simulation properties from input file
   int rows;
   int cols;
   int deadline;
   int max_load;
   std::vector<Drone> drones;
   std::vector<int> products;
   std::vector<Warehouse> warehouses;
   std::vector<Order> orders;
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* PROBLEM_H_ */
