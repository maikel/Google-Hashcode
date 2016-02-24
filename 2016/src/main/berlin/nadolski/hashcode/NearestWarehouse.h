#ifndef BERLIN_NADOLSKI_HASHCODE_NEARESTWAREHOUSE_H_
#define BERLIN_NADOLSKI_HASHCODE_NEARESTWAREHOUSE_H_

#include <set>

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
   std::list<Command> create_load_commands_for_product(Drone &drone, const Order &order, int product);
   std::list<Command> create_deliver_commands_from_load_commands(
         Drone &drone, Order &order, const std::list<Command> &load_commands);
   void push_back_load_command(std::list<Command> &commands, const Command &command);
   void push_back_deliver_command(std::list<Command> &commands, const Command &command);
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
   std::list<Drone*> find_helper_drones(Drone &drone);

   template <class compareT>
   std::set<int, compareT> find_product_indices_from_order(const Order &order, compareT compare) const;
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* MAIN_BERLIN_NADOLSKI_HASHCODE_NEARESTWAREHOUSE_H_ */
