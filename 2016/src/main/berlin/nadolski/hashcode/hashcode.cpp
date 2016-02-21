#include <fstream>
#include <memory>
#include <list>
#include <algorithm>
#include <cmath>
#include <cassert>

#include "Problem.h"
#include "Command.h"
#include "SimpleStrategy.h"

using namespace std;
using namespace berlin::nadolski;

namespace berlin {
namespace nadolski {
namespace hashcode {

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
         const Command &cmd = drone.command;
         switch (cmd.type) {
         case Command::DELIVER:
         {
            vector<int> products = p.orders[cmd.order].products;
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
         }
         case Command::LOAD:
         {
            assert(p.warehouses.size() > cmd.warehouse);
            vector<int> products = p.warehouses[cmd.warehouse].products;
            assert(products.size() > cmd.product);
            assert(products[cmd.product] >= cmd.value);
            products[cmd.product] -= cmd.value;
            break;
         }
         case Command::UNLOAD:
         {
            assert(p.warehouses.size() > cmd.warehouse);
            vector<int> products = p.warehouses[cmd.warehouse].products;
            assert(products.size() > cmd.product);
            products[cmd.product] += cmd.value;
            break;
         }
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

}
}
}

int main(int argc, char *argv[]) {
   if (argc == 1) {
      cerr << "Usage: " << argv[0] << " <filename>\n";
      return 0;
   }
   ifstream input(argv[1]);
   if (!input) {
      return -1;
   }
   cout << "Reading input file '" << argv[1] << "' ... ";
   hashcode::Problem problem(input);
   cout << "done\n";
   problem.print_parameter(cout);

   list<shared_ptr<hashcode::Strategy>> strats;
   strats.push_back(static_pointer_cast<hashcode::Strategy>(
         make_shared<hashcode::SimpleStrategy>(problem)));

   for (auto strategy : strats) {
      cout << "Generate commands with Strategy '" << strategy->name() << "' ... ";
      list<hashcode::Command> cmds = strategy->generate_commands();
      cout << "done. Produced " << cmds.size() << " commands.\n";
      if (!hashcode::validate(cmds, strategy->problem)) {
         cerr << "Could not validate the command list of strategy '"
               << strategy->name() << "'.\n";
      } else {
         cout << "Score: " << hashcode::simulate(cmds, strategy->problem) << endl;
      }
   }

   return 0;
}
