#include <cassert>
#include "SimpleStrategy.h"

using namespace std;

namespace berlin {
namespace nadolski {
namespace hashcode {

/**
 * return the first warehouse with an available product.
 * the first in order of how it is stored!
 */
int SimpleStrategy::get_warehouse_from_product(int p)
{
   for (int w  = 0; w < warehouses.size(); w++) {
      if (warehouses[w].products[p] > 0)
         return w;
   }
   // this should not be reached from being a well defined problem.
   assert(false);
   return -1;
}

/**
 * For each order assign one drone to deliver each entity of the
 * product. It is guaranteed that you can carry one entity of any product type.
 * To keep it simple transport one Product at a time.
 */
list<Command> SimpleStrategy::generate_commands()
{
   list<Command> commands;
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
            assert(warehouses[w].products[p] > 0);
            warehouses[w].products[p] -= 1;
            commands.push_back(Command(d, Command::load, w, p, 1));
            order.products[p] -= 1;
            commands.push_back(Command(d, Command::deliver, o, p, 1));
         }
      }
      d = (d + 1) % drones.size();
   }
   return commands;
}

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */
