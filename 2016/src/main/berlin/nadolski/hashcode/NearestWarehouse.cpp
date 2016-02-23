#include "NearestWarehouse.h"

#include <list>
#include <istream>
#include <map>
#include <set>
#include <algorithm>
#include <cassert>

using namespace std;

namespace berlin {
namespace nadolski {
namespace hashcode {

static inline float dist(const Drone &d, const Warehouse &w)
{
   return sqrt((d.x-w.x)*(d.x-w.x)+(d.y-w.y)*(d.y-w.y));
}

static vector<int> find_all_warehouses_with_product(const Drone &drone, int product, const vector<Warehouse> &warehouses) const
{
   vector<int> found_product;
   for (auto warehouse = warehouses.begin(); warehouse != warehouses.end(); warehouse++) {
      assert(warehouse->products.size() > product);
      if (warehouse->products[product] > 0)
         found_product.push_back(distance(warehouses.begin(), warehouse));
   }
   return found_product;
}

int NearestWarehouse::get_nearest_warehouse(const Drone &drone) const
{
   auto nearest = warehouses.begin();
   assert(nearest != warehouses.end());
   for (auto warehouse = nearest + 1; warehouse != warehouses.end(); warehouse++)
      if (dist(drone, *warehouse) < dist(drone, *nearest))
         nearest = warehouse;
   return std::distance(warehouses.begin(), nearest);
}

NearestWarehouse::NearestWarehouse(std::istream &in): Problem(in)
{
}

NearestWarehouse::~NearestWarehouse()
{
}

list<Command> NearestWarehouse::fill_drone_with_products_of_order(Drone &drone, Order &order)
{
   list<Command> commands;
   commands.append(Command(drone, Command::load, warehouse, product, value));
   int cost = 0;
   list<int> ordered_products_indices;
   for (auto product = order.products.begin(); product != order.products.end(); product++)
      if (*product > 0)
         ordered_products_indices.push_back(distance(order.products.begin(), product));

   for (auto product = order.products.begin(); product != order.products.end(); product++) {
      int ip = distance(order.products.begin(), product);
      // get all warehouses having the product and sort by distance
      vector<int> candidates = find_all_warehouses_with_product(drone, ip, warehouses);
      sort(candidates.begin(), candidates.end(), [this, &drone](int w, int v) {
         return dist(drone, warehouses[w]) < dist(drone, warehouses[v]);
      });
      for (auto nearest = candidates.begin(); nearest != candidates.end() && *product > 0; nearest++) {
         if (*product > local_warehouses[*nearest].products[ip]) {
            *product -= local_warehouses[*nearest].products[ip];
            local_warehouses[*nearest].products[ip] = 0;
         } else {
            local_warehouses[*nearest].products[ip] -= *product;
            *product = 0;
         }
         cost += ceil(dist(drone, local_warehouses[*nearest]));
      }
   }
}

list<Command> NearestWarehouse::generate_commands()
{
   list<Command> commands;

   while (orders.size() > 0) {
      // get the next free drone
      auto drone = min(drones.begin(), drones.end(), [](const Drone &d1, const Drone &d2) {
         return d1.busy < d2.busy;
      });
      // sort orders by cost
      auto compare_orders_by_drone_costT =
            [&drone](const Order &o1, const Order &o2) { return cost(*drone, o1) > cost(*drone, o2); };
      sort(orders.begin(), orders.end(), compare_orders_by_drone_costT);
      Order &best_order = orders.back();
      orders.pop_back();

      // find possible helping drones
      list<Drone> helper_drones = find_helper_drones(drone);s
      // while the active order has products to be delivered tell every helper drone
      // to load and deliver as much as they can
      auto helper = helper_drones.begin();
      while (find(best_order.products.begin(),
                  best_order.products.end(),
                  [](int p){return p > 0;}) != best_order.products.end()) {
         commands.splice(commands.end(), fill_drone_with_products_of_order(*helper, best_order));
         helper++;
         if (helper == helper_drones.end())
            helper = helper_drones.begin();
      }
   }
   return commands;
}

int NearestWarehouse::cost(const Drone &drone, const Order &order) const
{
   // for each product get index of nearest warehouse
   vector<Warehouse> local_warehouses(warehouses);
   vector<int> order_products(order.products);
   int cost = 0;
   for (auto product = order_products.begin(); product != order_products.end(); product++) {
      int ip = distance(order_products.begin(), product);
      vector<int> candidates = find_all_warehouses_with_product(drone, ip, local_warehouses);
      sort(candidates.begin(), candidates.end(), [&local_warehouses, &drone](int w, int v){
         return dist(drone, local_warehouses[w]) < dist(drone, local_warehouses[v]);
      });
      for (auto nearest = candidates.begin(); nearest != candidates.end() && *product > 0; nearest++) {
         if (*product > local_warehouses[*nearest].products[ip]) {
            *product -= local_warehouses[*nearest].products[ip];
            local_warehouses[*nearest].products[ip] = 0;
         } else {
            local_warehouses[*nearest].products[ip] -= *product;
            *product = 0;
         }
         cost += ceil(dist(drone, local_warehouses[*nearest]));
      }
   }
   return cost;
}

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */
