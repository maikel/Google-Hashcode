#include "Strategy.h"

#include <cmath>
#include <cassert>
#include <algorithm>

using namespace std;

namespace berlin {
namespace nadolski {
namespace hashcode {

Strategy::Strategy(const Problem &p) :
      problem(p) {

}

Strategy::~Strategy() {
   // TODO Auto-generated destructor stub
}

/**
 * Validate a given command list and problem set. Essentially do the simulation
 * and check all rules every step.
 */
bool validate(const list<Command> &commands, const Problem &problem) {
   return true;
}

int simulate_perform_commands(int t, int T, const Problem &p) {
   int score = 0;
   // perform pending commands and increase score if neccessary
   for (Drone drone : p.drones) {
      if (drone.busy == 1) {
         vector<int> &products;
         const Command &cmd = drone.command;
         switch (cmd.type) {
         case Command::DELIVER:
            products = p.orders[cmd.order].products;
            auto it = find(products.begin(), products.end(), cmd.product);
            assert(it != products.end());
            *it -= cmd.value;
            assert(*it >= 0);
            // check if we completed the order
            bool completed = true;
            for (int i = 0; i < products.size(); i++)
               if (products[i] > 0)
                  completed = false;
            if (completed) {
               assert(ceil(float(T - t * 100) / T) > 0);
               score += ceil(float(T - t * 100) / T);
            }
            break;
         case Command::LOAD:
            assert(p.warehouses.size() > cmd.warehouse);
            products = p.warehouses[cmd.warehouse].products;
            assert(products.size() > cmd.product);
            assert(products[cmd.product] >= cmd.value);
            products[cmd.product] -= cmd.value;
         case Command::UNLOAD:
            assert(p.warehouses.size() > cmd.warehouse);
            products = p.warehouses[cmd.warehouse].products;
            assert(products.size() > cmd.product);
            products[cmd.product] += cmd.value;
         }
      }
      if (drone.busy > 0)
         drone.busy -= 1;
   }
   return score;
}

/**
 * Simulate the problem and calculate the total score of a given solution.
 */
int simulate(list<Command> commands, Problem p) {
   int score = 0;
   int T = p.deadline;

   for (int t = 0; t < T; t++) {
      // apply to each drone a command and remove given commands from list
      for (const Command &cmd : commands) {
         if (p.drones[cmd.drone].busy == 0) {
            assert(p.drones.size() > cmd.drone);
            Drone &drone = p.drones[cmd.drone];
            drone.command = cmd;
            switch (cmd.type) {
            case Command::WAIT:
               drone.busy = cmd.value;
               break;
            case Command::LOAD:
               break;
            }
         }
         commands.remove(cmd);
      }
      score += simulate_perform_commands(t, T, p);
   }
   return score;
}

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */
