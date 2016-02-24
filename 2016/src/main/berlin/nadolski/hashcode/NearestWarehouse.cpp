#include "NearestWarehouse.h"

#include <list>
#include <istream>
#include <map>
#include <set>
#include <algorithm>
#include <stack>
#include <cassert>

using namespace std;

namespace berlin {
namespace nadolski {
namespace hashcode {

/**
 * Euclidean distances
 */
static inline float
dist(const Drone &d, const Order &o) noexcept
{
   return sqrt((d.x-o.x)*(d.x-o.x)+(d.y-o.y)*(d.y-o.y));
}
static inline float
dist(const Drone &d, const Warehouse &w) noexcept
{
   return sqrt((d.x-w.x)*(d.x-w.x)+(d.y-w.y)*(d.y-w.y));
}

static list<Warehouse*>
find_all_warehouses_with_product(const Drone &drone, int product, vector<Warehouse> &warehouses)
{
   assert(warehouses.size() > 0);
   list<Warehouse*> warehouses_with_product;
   for (Warehouse &warehouse : warehouses)
      if (warehouse.products[product] > 0)
         warehouses_with_product.push_back(&warehouse);
   return warehouses_with_product;
}

NearestWarehouse::NearestWarehouse(std::istream &in): Problem(in)
{
}

NearestWarehouse::~NearestWarehouse()
{
}

template <class compareT>
std::set<int, compareT>
NearestWarehouse::find_product_indices_from_order(const Order &order, compareT compare) const
{
   set<int, compareT> ordered_products(compare);
   for (int iord = 0; iord < order.products.size(); iord++)
      if (order.products[iord] > 0)
         ordered_products.insert(iord);
   return ordered_products;
}

void NearestWarehouse::push_back_load_command(
      list<Command> &commands, const Command &command, Order &order)
{
   assert(command.type == Command::load);
   assert(warehouses[command.warehouse].products[command.product] >= command.value);
   assert(drones[command.drone].load+product_weights[command.product]*command.value <= max_load);
   assert(order.products.size() > command.product);
   order.products[command.product] -= command.value;
   assert(order.products[command.product] >= 0);
   warehouses[command.warehouse].products[command.product] -= command.value;
   drones[command.drone].load += product_weights[command.product]*command.value;
   assert(drones[command.drone].load <= max_load);
   drones[command.drone].busy += ceil(dist(drones[command.drone], warehouses[command.warehouse]))+1;
   drones[command.drone].x = warehouses[command.warehouse].x;
   drones[command.drone].y = warehouses[command.warehouse].y;
   commands.push_back(command);
}

void NearestWarehouse::push_back_deliver_command(
      list<Command> &commands, const Command &command)
{
   assert(command.type == Command::deliver);
   assert(orders[command.order].products[command.product] >= command.value);
   orders[command.order].products[command.product] -= command.value;
   drones[command.drone].load -= product_weights[command.product]*command.value;
   drones[command.drone].busy += ceil(dist(drones[command.drone], orders[command.order]))+1;
   drones[command.drone].x = orders[command.order].x;
   drones[command.drone].y = orders[command.order].y;
   commands.push_back(command);
}

list<Command> NearestWarehouse::create_load_commands_for_product(Drone &drone, Order &order, int product)
{
   list<Command> commands;
   list<Warehouse*> warehouses_with_product = find_all_warehouses_with_product(drone, product, warehouses);
   warehouses_with_product.sort([&drone](Warehouse *w, Warehouse *v) {
      return dist(drone, *w) < dist(drone, *v);
   });
   assert(warehouses_with_product.size() > 0);
   for (Warehouse *warehouse : warehouses_with_product) {
      assert(warehouse != nullptr);
      assert(warehouse->products[product] > 0);
      assert(order.products[product] > 0);
      assert(drone.load < max_load);
      int num_product_from_warehouse = min(order.products[product], warehouse->products[product]);
      if (max_load >= drone.load + num_product_from_warehouse*product_weights[product]) {
         push_back_load_command(commands,
               Command(drone.id, Command::load, warehouse->id, product, num_product_from_warehouse), order);
      } else {
         num_product_from_warehouse = (max_load-drone.load)/product_weights[product];
         assert(num_product_from_warehouse > 0);
         assert(max_load >= drone.load + num_product_from_warehouse*product_weights[product]);
         push_back_load_command(commands,
               Command(drone.id, Command::load, warehouse->id, product, num_product_from_warehouse), order);
      }
      if (max_load < drone.load+product_weights[product] || order.products[product] == 0)
         break;
   }
   assert(commands.size() > 0);
   return commands;
}

list<Command> NearestWarehouse::create_deliver_commands_from_load_commands(
      Drone &drone, Order &order, const list<Command> &load_commands)
{
   list<Command> deliver_commands;
   for (Command load : load_commands) {
      push_back_deliver_command(deliver_commands,
            Command(drone.id, Command::deliver, order.id, load.product, load.value));
   }
   return deliver_commands;
}

list<Command> NearestWarehouse::fill_drone_with_products_of_order(Drone &drone, Order &order)
{
   list<Command> commands;
   int cost = 0;
   auto compare_with_weightT = [this] (int p1, int p2) {
      return product_weights[p1] < product_weights[p2];
   };
   auto product_indices_sorted = find_product_indices_from_order(order, compare_with_weightT);
   auto product = product_indices_sorted.begin();
   assert(product != product_indices_sorted.end());
   assert(*product < product_weights.size());
   list<Command> load_commands;
   while (product != product_indices_sorted.end() && drone.load+product_weights[*product] < max_load) {
      assert(order.products.size() > *product);
      assert(order.products[*product] > 0);
      list<Command> tmp_load_commands = create_load_commands_for_product(drone, order, *product);
      load_commands.splice(load_commands.end(), tmp_load_commands);
      product++;
   }
   list<Command> deliver_commands = create_deliver_commands_from_load_commands(drone, order, load_commands);
   commands.splice(commands.end(), load_commands);
   commands.splice(commands.end(), deliver_commands);
   return commands;
}

list<Drone*> NearestWarehouse::find_helper_drones(Drone &drone)
{
   list<Drone*> helpers;
   for (Drone &d : drones)
      if (d.x == drone.x && d.y == drone.y)
         helpers.push_back(&d);
   return helpers;
}

list<Command> NearestWarehouse::generate_commands()
{
   list<Command> commands;
   list<Order> orders;
   copy(this->orders.begin(), this->orders.end(), back_inserter(orders));
   int count = 1;

   while (orders.size() > 0) {
      // get the next free drone
      auto drone = drones.begin();
      for (auto d = drones.begin(); d != drones.end(); d++)
         if (d->busy < drone->busy)
            drone = d;

      // sort orders by cost
      auto compare_orders_by_drone_costT =
            [this,&drone](const Order &o1, const Order &o2) { return cost(*drone, o1) > cost(*drone, o2); };
      auto best_order = min_element(orders.begin(), orders.end(), compare_orders_by_drone_costT);
      cerr << "Generating commands for order " << best_order->id << " (" << float(count++)/this->orders.size()*100 << "%)\n";
      // find possible helping drones
      list<Drone*> helper_drones = find_helper_drones(*drone);
      // while the active order has products to be delivered tell every helper drone
      // to load and deliver as much as they can
      auto helper = helper_drones.begin();
      while (find_if(best_order->products.begin(),best_order->products.end(),
            [](int product){return product > 0;}) != best_order->products.end()) {
         list<Command> commands_product = fill_drone_with_products_of_order(**helper, *best_order);
         commands.splice(commands.end(), commands_product);
         helper++;
         if (helper == helper_drones.end())
            helper = helper_drones.begin();
      }
      orders.erase(best_order);
   }
   return commands;
}

/**
 * This cost functions determines how long it takes in total number of turns
 * to complete a given order.
 */
int NearestWarehouse::cost(const Drone &drone, const Order &order) const
{
   vector<Warehouse> local_warehouses(warehouses);
   vector<int> order_products(order.products);
   int cost = 0;
   for (auto product = order_products.begin(); product != order_products.end(); product++) {
      if (*product > 0) {
         int ip = distance(order_products.begin(), product);
         list<Warehouse*> whcandidates = find_all_warehouses_with_product(drone, ip, local_warehouses);
         whcandidates.sort([&drone](Warehouse *w, Warehouse *v){
            return dist(drone, *w) < dist(drone, *v);
         });
         auto test = whcandidates.begin();
         for (auto warehouse = whcandidates.begin(); warehouse != whcandidates.end() && *product > 0; warehouse++) {
            if (*product > (*warehouse)->products[ip]) {
               *product -= (*warehouse)->products[ip];
               (*warehouse)->products[ip] = 0;
            } else {
               (*warehouse)->products[ip] -= *product;
               *product = 0;
            }
            cost += ceil(dist(drone, **warehouse));
         }
      }
   }
   return cost;
}

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */
