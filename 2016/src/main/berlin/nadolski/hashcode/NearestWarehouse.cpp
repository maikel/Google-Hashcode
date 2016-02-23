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

static inline float dist(const Drone &d, const Order &w)
{
   return sqrt((d.x-w.x)*(d.x-w.x)+(d.y-w.y)*(d.y-w.y));
}

static inline float dist(const Drone &d, const Warehouse &w)
{
   return sqrt((d.x-w.x)*(d.x-w.x)+(d.y-w.y)*(d.y-w.y));
}

static vector<int> find_all_warehouses_with_product(const Drone &drone, int product, const vector<Warehouse> &warehouses)
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

template <class T>
size_t find_index(const vector<T> &vs, const T &value)
{
   int i = 0;
   for (i = 0; i < vs.size(); i++)
      if (vs[i] == value)
         return i;
   return i;
}

list<Command> NearestWarehouse::fill_drone_with_products_of_order(Drone &drone, Order &order)
{
   list<Command> commands;
   int cost = 0;
   auto compare_by_weightT = [this](int p1, int p2) {
      assert(product_weights.size() > p1);
      assert(product_weights.size() > p2);
      return product_weights[p1] < product_weights[p2];
   };
   set<int, decltype(compare_by_weightT)> ordered_products(compare_by_weightT);
   for (auto p = order.products.begin(); p != order.products.end(); p++)
      if (*p > 0)
         ordered_products.insert(distance(order.products.begin(), p));
   auto product = ordered_products.begin();
   assert(product != ordered_products.end());
   assert(order.products.size() > *product);
   assert(order.products[*product] > 0);

   // get all warehouses having the product and sort by distance
   vector<int> candidates = find_all_warehouses_with_product(drone, *product, warehouses);
   sort(candidates.begin(), candidates.end(), [this, &drone](int w, int v) {
      return dist(drone, warehouses[w]) < dist(drone, warehouses[v]);
   });

   assert(order.products.size() > *product);
   assert(order.products[*product] > 0);
   // add load commands for this product as much as you can
   for (auto nearestwh = candidates.begin(); nearestwh != candidates.end() && order.products[*product] > 0; nearestwh++) {
      int num_of_product = 0;
      while (drone.load + product_weights[*product] < max_load
            && order.products[*product] > 0 && warehouses[*nearestwh].products[*product] > 0) {
         num_of_product++;
         drone.load += product_weights[*product];
         order.products[*product]--;
         warehouses[*nearestwh].products[*product]--;
      }
      if (num_of_product > 0) {
         int d = distance(drones.begin(), find(drones.begin(), drones.end(), drone));
         int o = distance(orders.begin(), find(orders.begin(), orders.end(), order));
         assert(drones.size() > d);
         assert(orders.size() > o);
         commands.push_back(Command(d, Command::load, *nearestwh, *product, num_of_product));
         cout << commands.back() << endl;
         commands.push_back(Command(d, Command::deliver, o, *product, num_of_product));
         cout << commands.back() << endl;
         drone.busy += dist(drone, order);
         break;
      }
   }

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
   vector<Order*> orders;
   for_each(this->orders.begin(), this->orders.end(), [&orders](Order &o) {
      orders.push_back(&o);
   });

   while (orders.size() > 0) {
      // get the next free drone
      auto drone = drones.begin();
      for (auto d = drones.begin(); d != drones.end(); d++)
         if (d->busy < drone->busy)
            drone = d;

      // sort orders by cost
      auto compare_orders_by_drone_costT =
            [this,drone](const Order *o1, const Order *o2) { return cost(*drone, *o1) > cost(*drone, *o2); };
      sort(orders.begin(), orders.end(), compare_orders_by_drone_costT);
      Order *best_order = orders.back();

      // find possible helping drones
      list<Drone*> helper_drones = find_helper_drones(*drone);
      // while the active order has products to be delivered tell every helper drone
      // to load and deliver as much as they can
      auto helper = helper_drones.begin();
      while (find_if(best_order->products.begin(),
                  best_order->products.end(),
                  [](int p){return p > 0;}) != best_order->products.end()) {

         commands.splice(commands.end(), fill_drone_with_products_of_order(**helper, *best_order));
         helper++;
         if (helper == helper_drones.end())
            helper = helper_drones.begin();
      }
      orders.pop_back();
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
