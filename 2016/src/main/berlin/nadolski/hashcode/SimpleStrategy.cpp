#include <cassert>
#include "SimpleStrategy.h"

using namespace std;

namespace berlin {
namespace nadolski {
namespace hashcode {

/**
 * return the first warehouse with an available product.
 */
int SimpleStrategy::get_warehouse_from_product(int p) {
   vector<Warehouse> warehouses = problem.warehouses;
   for (int w  = 0; w < warehouses.size(); w++) {
      if (warehouses[w].products[p] > 0)
         return w;
   }
   // this should not be reached from being a well defined problem.
   assert(false);
}

/**
 * For each order assign one drone to deliver each entity of the
 * product. It is guaranteed that you can carry one entity of any product type.
 * To keep it simple transport one Product at a time.
 */
list<Command> SimpleStrategy::generate_commands() {
   list<Command> commands;
   vector<Order> &orders = problem.orders;
   vector<Drone> &drones = problem.drones;
   int d = 0;
   for (int o = 0; o < orders.size(); o++) {
      Order &order = orders[o];
      for (int p = 0; p < order.products.size(); p++) {
         // if the order requests a product, look for the next available
         // entity, load and remove its capacity from the warehouse
         // and deliver it to the client
         assert(order.products.size() > p);
         while (order.products[p] > 0) {
            int w = get_warehouse_from_product(p);
            assert(problem.warehouses[w].products[p] > 0);
            problem.warehouses[w].products[p] -= 1;
            commands.push_back(Command(d, Command::LOAD, w, p, 1));
            order.products[p] -= 1;
            commands.push_back(Command(d, Command::DELIVER, o, p, 1));
         }
      }
      d = (d + 1) % drones.size();
   }
   return commands;
}

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */
