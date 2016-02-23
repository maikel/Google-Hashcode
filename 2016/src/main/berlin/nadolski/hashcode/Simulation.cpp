#include "Simulation.h"

#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;

namespace berlin {
namespace nadolski {
namespace hashcode {

static int perform_commands(int t, int T, Problem &p) {
   int score = 0;
   // perform pending commands and increase score if neccessary
   for (int d = 0; d < p.drones.size(); d++) {
      Drone &drone = p.drones[d];
      if (drone.busy == 1) {
         cout << "Drone " << d << " performs its assinged command: "
               << drone.command << endl;
         const Command &cmd = drone.command;
         switch (cmd.type) {
         case Command::DELIVER:
         {
            assert(p.orders.size() > cmd.order);
            vector<int> &products = p.orders[cmd.order].products;
            assert(products.size() > cmd.product);
            products[cmd.product] -= cmd.value;
            assert(products[cmd.product] >= 0);
            // set new coordinates for drone
            drone.x = p.orders[cmd.order].x;
            drone.y = p.orders[cmd.order].y;
            // check if we completed the order
            bool completed = true;
            for (int i = 0; i < products.size(); i++)
               if (products[i] > 0)
                  completed = false;
            if (completed) {
               assert(ceil(float((T - t) * 100) / T) > 0);
               score += ceil(float((T - t) * 100) / T);
               cout << "Completing order " << cmd.order << " and scoring "
                     << score << " many Points.\n";
            }
            break;
         }
         case Command::LOAD:
         {
            assert(p.warehouses.size() > cmd.warehouse);
            vector<int> &products = p.warehouses[cmd.warehouse].products;
            assert(products.size() > cmd.product);
            assert(products[cmd.product] >= cmd.value);
            assert(p.max_load >= p.products[cmd.product]*cmd.value);
            products[cmd.product] -= cmd.value;
            // set new coordinates for drone
            drone.x = p.warehouses[cmd.warehouse].x;
            drone.y = p.warehouses[cmd.warehouse].y;
            break;
         }
         case Command::UNLOAD:
         {
            assert(p.warehouses.size() > cmd.warehouse);
            vector<int> &products = p.warehouses[cmd.warehouse].products;
            assert(products.size() > cmd.product);
            products[cmd.product] += cmd.value;
            // set new coordinates for drone
            drone.x = p.warehouses[cmd.warehouse].x;
            drone.y = p.warehouses[cmd.warehouse].y;
            break;
         }
         default:
            break;
         }
      }
      if (drone.busy > 0) {
         drone.busy -= 1;
      }
   }
   return score;
}

float distance(int x1, int y1, int x2, int y2) {
   return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

static list<Command> assign_commands(list<Command> &commands, Problem &p) {
   list<Command> queue = commands;
   commands = list<Command>();
   while (queue.size() > 0) {
      Command cmd = queue.front();
      queue.pop_front();
      assert(p.drones.size() > cmd.drone);
      if (p.drones[cmd.drone].busy == 0) {
         cout << "Drone " << cmd.drone << " recieves a new command "
               << cmd << endl;
         assert(p.drones.size() > cmd.drone);
         Drone &drone = p.drones[cmd.drone];
         drone.command = cmd;
         switch (cmd.type) {
         case Command::WAIT:
            drone.busy = cmd.value;
            break;
         case Command::LOAD:
            assert(p.warehouses.size() > cmd.warehouse);
            drone.busy = ceil(distance(
               p.warehouses[cmd.warehouse].x,
               p.warehouses[cmd.warehouse].y,
               drone.x, drone.y)) + 1;
            if (drone.busy > 1) {
               cout << "Drone " << cmd.drone << " has to fly from grid cell "
                     << "(" << drone.x << "," << drone.y << ") to the grid cell "
                     << "(" << p.warehouses[cmd.warehouse].x
                     << "," << p.warehouses[cmd.warehouse].y << ") and will need "
                     << drone.busy-1 << " turns to do so.\n";
            }
            break;
         case Command::DELIVER:
            assert(p.orders.size() > cmd.order);
            drone.busy = ceil(distance(
               p.orders[cmd.order].x, p.orders[cmd.order].y,
               drone.x, drone.y)) + 1;
            if (drone.busy > 1) {
               cout << "Drone " << cmd.drone << " has to fly from grid cell "
                     << "(" << drone.x << "," << drone.y << ") to the grid cell "
                     << "(" << p.orders[cmd.order].x
                     << "," << p.orders[cmd.order].y << ") and will need "
                     << drone.busy-1 << " turns to do so.\n";
            }
            break;
         default:
            assert(false);
            break;
         }
      } else {
         commands.push_back(cmd);
      }
   }
   return commands;
}


Simulation::Simulation(std::istream &in) :
      problem(in) {

}

int Simulation::run(list<Command> commands) {
   int score = 0;
   int T = problem.deadline;

   for (int t = 0; t < T; t++) {
      cout << "time step t = " << t << endl;
      commands = assign_commands(commands, problem);
      score += perform_commands(t, T, problem);
   }
   return score;
}

Simulation::~Simulation() {

}

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */
