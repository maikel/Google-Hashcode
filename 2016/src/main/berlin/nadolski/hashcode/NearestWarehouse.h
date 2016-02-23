#ifndef BERLIN_NADOLSKI_HASHCODE_NEARESTWAREHOUSE_H_
#define BERLIN_NADOLSKI_HASHCODE_NEARESTWAREHOUSE_H_

#include "Problem.h"
#include "Strategy.h"

namespace berlin {
namespace nadolski {
namespace hashcode {

class NearestWarehouse: public Problem, public Strategy {
public:
   NearestWarehouse(std::istream &in);
   virtual ~NearestWarehouse();

   /*
    * return an "optimized" solution
    */
   std::list<Command> generate_commands();

private:
   /*
    * get the first best warehouse which contains a certain product.
    * order is given by how it is stored.
    */
   int get_nearest_warehouse(const Drone &drone) const;

   /*
    * returns a weight for an order depending on avaiable resources and
    * distances to the warehouses
    */
   int cost(const Drone &drone, const Order &order) const;

   std::list<Command> fill_drone_with_products_of_order(Drone &drone, Order &order);
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* MAIN_BERLIN_NADOLSKI_HASHCODE_NEARESTWAREHOUSE_H_ */
