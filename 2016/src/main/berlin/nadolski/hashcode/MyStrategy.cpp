#include "MyStrategy.h"

#include <vector>
#include <list>
#include <algorithm>  // min_element
#include <functional> // greater
#include <iterator>   // begin, end, distance
#include <numeric>
#include <cassert>

using namespace std;

namespace berlin {
namespace nadolski {
namespace hashcode {

namespace {

int cost(const Order &order, const ProblemState &state);
list<Order*> get_list_orders_from_state(ProblemState &state);
list<Warehouse*> find_warehouses_with_products_of_order(const Order &order, ProblemState &state);
int take_products_for_order(Warehouse &warehouse, Order &order, const vector<int> &weights);
inline list<Order*>::iterator find_order_with_minimal_cost(list<Order*> &orders, const ProblemState &state);
inline Drone &find_least_busy_drone(ProblemState &state);
inline bool is_least_busy_drone(const Drone &drone, const ProblemState &state);
inline bool is_delivered(const Order &order);

template <typename A, typename B>
   // requires Vector2<A>, Vector2<B>
float dist(const A &a, const B &b)
{
   return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

}

MyStrategy::MyStrategy(const ProblemState &s)
: state(s)
{
}

MyStrategy::MyStrategy(istream &in)
: state(in)
{
}

vector<Command> MyStrategy::generate_commands()
{
   vector<Command> commands;
   list<Order*> orders = get_list_orders_from_state(state);
   assert(orders.size() > 0);
   while (orders.size() > 0) {
      Drone &drone = find_least_busy_drone(state);
      assert(is_least_busy_drone(drone, state));
      list<Order*>::iterator order = find_order_with_minimal_cost(orders, state);
      cout << "order " << (*order)->id << " has minimal cost: " << cost(**order, state) << endl;
//      vector<Command> new_commands = create_commands(drone, **order, state); // every argument gets changed here
//      assert(load_deliver_zero_sum(new_commands));
//      copy(new_commands.begin(), new_commands.end(), back_inserter(commands));
      if (is_delivered(**order))
         orders.erase(order);
   }
   return commands;
}

namespace {

list<Order*> get_list_orders_from_state(ProblemState &state)
{
   list<Order*> orders;
   for (Order &order : state.orders)
      orders.push_back(&order);
   return orders;
}

inline list<Order*>::iterator find_order_with_minimal_cost(list<Order*> &orders, const ProblemState &state)
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
   ProblemState local_state(state);
   Order &local_order = local_state.orders[order.id];
   list<Warehouse*> warehouses = find_warehouses_with_products_of_order(local_order, local_state);
   warehouses.sort([&local_order](Warehouse *w, Warehouse *v){ return dist(local_order, *w) < dist(local_order, *v); });
   for (Warehouse *w : warehouses) {
      int diff = take_products_for_order(*w, local_order, state.product_weights);
      cost += ceil(float(diff)/state.max_load)*ceil(dist(local_order, *w));
      if (is_delivered(local_order))
         break;
   }
   return cost;
}

list<Warehouse*> find_warehouses_with_products_of_order(const Order &order, ProblemState &state)
{
   list<Warehouse*> found;
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

int take_products_for_order(Warehouse &warehouse, Order &order, const vector<int> &weights)
{
   int diff = 0;
   for (int p = 0; p < order.products.size(); p++) {
      assert(warehouse.products.size() > p);
      if (order.products[p] > 0 && warehouse.products[p] > 0) {
         if (order.products[p] > warehouse.products[p]) {
            order.products[p] -= warehouse.products[p];
            diff += warehouse.products[p]*weights[p];
            warehouse.products[p] = 0;
         } else {
            warehouse.products[p] -= order.products[p];
            diff += order.products[p]*weights[p];
            order.products[p] = 0;
         }
      }
   }
   return diff;
}

} // namespace
} // namespace hashcode
} // namespace nadolski
} // namespace berlin
