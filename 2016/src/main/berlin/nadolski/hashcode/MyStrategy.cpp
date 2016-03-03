#include "MyStrategy.h"

#include <vector>
#include <list>
#include <algorithm>  // min_element
#include <functional> // greater
#include <iterator>   // begin, end, distance
#include <numeric>
#include <cassert>
#include <map>

namespace berlin {
namespace nadolski {
namespace hashcode {

namespace {

int cost(const Order &order, const ProblemState &state);
std::list<Order*> get_list_orders_from_state(ProblemState &state);
std::list<const Warehouse*> find_warehouses_with_products_of_order(const Order &order, const ProblemState &state);
std::list<Warehouse*> find_warehouses_with_products_of_order(const Order &order, ProblemState &state);
int take_products_for_order(const Warehouse &warehouse, Order &order, const std::vector<int> &weights);
inline std::list<Order*>::iterator find_order_with_minimal_cost(std::list<Order*> &orders, const ProblemState &state);
inline Drone &find_least_busy_drone(ProblemState &state);
inline bool is_least_busy_drone(const Drone &drone, const ProblemState &state);
inline bool is_delivered(const Order &order);
bool load_deliver_zero_sum(const std::vector<Command> &commands);
std::vector<Command> create_commands(Drone &drone, Order &order, ProblemState &state);

template <typename A, typename B>
float dist(const A &a, const B &b)
{
   return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

}

MyStrategy::MyStrategy(const ProblemState &s)
: state(s)
{
}

MyStrategy::MyStrategy(std::istream &in)
: state(in)
{
}

std::vector<Command> MyStrategy::generate_commands()
{
   std::vector<Command> commands;
   std::list<Order*> orders = get_list_orders_from_state(state);
   assert(orders.size() > 0);
   while (orders.size() > 0) {
      Drone &drone = find_least_busy_drone(state);
      assert(is_least_busy_drone(drone, state));
      std::list<Order*>::iterator order = find_order_with_minimal_cost(orders, state);
      std::vector<Command> new_commands = create_commands(drone, **order, state); // every argument gets changed here
      assert(load_deliver_zero_sum(new_commands));
      copy(new_commands.begin(), new_commands.end(), back_inserter(commands));
      std::cerr << "drone " << drone.id << " delivers for order " << (*order)->id << std::endl;
      if (is_delivered(**order)) {
         orders.erase(order);
         std::cerr << "order " << (*order)->id
              << " completed. (" << 100-float(orders.size())/state.orders.size()*100 << "%)\n";
      }
   }
   return commands;
}

namespace {

std::list<Order*> get_list_orders_from_state(ProblemState &state)
{
   std::list<Order*> orders;
   for (Order &order : state.orders)
      orders.push_back(&order);
   return orders;
}

inline std::list<Order*>::iterator find_order_with_minimal_cost(std::list<Order*> &orders, const ProblemState &state)
{
   return min_element(orders.begin(), orders.end(),[&state](const Order *o1, const Order *o2) {
         return cost(*o1, state) < cost(*o2, state);
   });
}

inline bool is_delivered(const Order &order)
{
   return accumulate(order.products.begin(), order.products.end(), true, [](bool b, int p){
      return b && p == 0;
   });
}

inline bool is_least_busy_drone(const Drone &drone, const ProblemState &state)
{
   return accumulate(state.drones.begin(), state.drones.end(), true, [&drone](bool b, const Drone &d) {
      return b && drone.busy <= d.busy;
   });
}

inline Drone &find_least_busy_drone(ProblemState &state)
{
   return *min_element(state.drones.begin(), state.drones.end(), [](const Drone &d1, const Drone &d2){
      return d1.busy < d2.busy;
   });
}

int cost(const Order &order, const ProblemState &state)
{
   int cost = 0;
   Order local_order(order);
   std::list<const Warehouse*> warehouses = find_warehouses_with_products_of_order(local_order, state);
   warehouses.sort([&local_order](const Warehouse *w, const Warehouse *v){ return dist(local_order, *w) < dist(local_order, *v); });
   for (const Warehouse *w : warehouses) {
      int diff = take_products_for_order(*w, local_order, state.product_weights);
      cost += ceil(float(diff)/state.max_load)*ceil(dist(local_order, *w));
      if (is_delivered(local_order))
         break;
   }
   return cost;
}

std::list<Warehouse*> find_warehouses_with_products_of_order(const Order &order, ProblemState &state)
{
   std::list<Warehouse*> found;
   for (Warehouse &warehouse : state.warehouses) {
      for (int p = 0; p < warehouse.products.size(); p++) {
         assert(order.products.size() > p);
         if (warehouse.products[p] > 0 && order.products[p] > 0) {
            found.push_back(&warehouse);
            break;
         }
      }
   }
   return found;
}

std::list<const Warehouse*> find_warehouses_with_products_of_order(const Order &order, const ProblemState &state)
{
   std::list<const Warehouse*> found;
   for (const Warehouse &warehouse : state.warehouses) {
      for (int p = 0; p < warehouse.products.size(); p++) {
         assert(order.products.size() > p);
         if (warehouse.products[p] > 0 && order.products[p] > 0) {
            found.push_back(&warehouse);
            break;
         }
      }
   }
   return found;
}

int take_products_for_order(const Warehouse &warehouse, Order &order, const std::vector<int> &weights)
{
   int diff = 0;
   for (int p = 0; p < order.products.size(); p++) {
      assert(warehouse.products.size() > p);
      if (order.products[p] > 0 && warehouse.products[p] > 0) {
         if (order.products[p] > warehouse.products[p]) {
            order.products[p] -= warehouse.products[p];
            diff += warehouse.products[p]*weights[p];
//            warehouse.products[p] = 0;
         } else {
//            warehouse.products[p] -= order.products[p];
            diff += order.products[p]*weights[p];
            order.products[p] = 0;
         }
      }
   }
   return diff;
}

bool load_deliver_zero_sum(const std::vector<Command> &commands)
{
   std::map<int, int> products;
   for (const Command &command : commands) {
      if (command.type == Command::load)
         products[command.product] += command.value;
      else if (command.type == Command::deliver)
         products[command.product] -= command.value;
   }
   return accumulate(products.begin(), products.end(), true, [](bool b, std::pair<int,int> p){ return b && p.second == 0; });
}

std::vector<int> find_shared_products_sorted_by_weight(
      const Order &order, const Warehouse &warehouse, const std::vector<int> &weights)
{
   std::vector<int> shared_products;
   for (int p = 0; p < order.products.size(); p++)
      if (order.products[p] > 0 && warehouse.products[p] > 0)
         shared_products.push_back(p);
   std::sort(shared_products.begin(), shared_products.end(), [&weights](int p1, int p2) {
      return weights[p1] < weights[p2];
   });
   return shared_products;
}

std::vector<Command> fill_drone_with_warehouse_products(
      Drone &drone, Order &order, Warehouse &warehouse, const ProblemState &state)
{
   std::vector<Command> commands;
   auto compare_by_weightT = [&state](int p1, int p2) {
      return state.product_weights[p1] < state.product_weights[p2];
   };
   std::vector<int> shared_products = find_shared_products_sorted_by_weight(order, warehouse, state.product_weights);
   for (int product : shared_products) {
      int weight = state.product_weights[product];
      if (drone.load+weight > state.max_load)
         break;
      int value = std::min(
            (state.max_load-drone.load)/weight,
            std::min(order.products[product], warehouse.products[product])
      );
      order.products[product] -= value;
      warehouse.products[product] -= value;
      drone.load += value*weight;
      drone.x = warehouse.x;
      drone.y = warehouse.y;
      drone.busy += ceil(dist(drone, warehouse));
      assert(order.products[product] >= 0);
      assert(warehouse.products[product] >= 0);
      assert(drone.load <= state.max_load);
      commands.push_back(Command(drone.id, Command::load, warehouse.id, product, value));
   }
   return commands;
}

std::vector<Command> deliver_from_load(Drone &drone, Order &order, const std::vector<Command> &load_commands)
{
   std::vector<Command> deliver_commands;
   std::map<int, int> product_vals;
   for (const Command &command : load_commands) {
      assert(command.type == Command::load);
      product_vals[command.product] += command.value;
   }
   for (const std::pair<int,int> &p : product_vals) {
      drone.busy += ceil(dist(drone, order));
      drone.x = order.x;
      drone.y = order.y;
      drone.load = 0;
      deliver_commands.push_back(Command(drone.id, Command::deliver, order.id, p.first, p.second));
   }
   return deliver_commands;
}

std::vector<Command> create_commands(Drone &drone, Order &order, ProblemState &state)
{
   std::vector<Command> commands;
   std::list<Warehouse*> warehouses = find_warehouses_with_products_of_order(order, state);
   warehouses.sort([&drone](Warehouse *w, Warehouse *v) { return dist(drone, *w) < dist(drone, *v); });
   auto warehouse = warehouses.begin();
   std::vector<Command> load_commands = fill_drone_with_warehouse_products(drone, order, **warehouse, state);
   std::vector<Command> deliver_commands = deliver_from_load(drone, order, load_commands);
   std::copy(load_commands.begin(), load_commands.end(), back_inserter(commands));
   std::copy(deliver_commands.begin(), deliver_commands.end(), back_inserter(commands));
   return commands;
}

} // namespace
} // namespace hashcode
} // namespace nadolski
} // namespace berlin
