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

std::stringstream &get_next_line(stringstream &line, istream &in) {
   if (!in) {
      cerr << "Some input error occured.\n";
      exit(-1);
   }
   string tmp;
   getline(in, tmp);
   line.clear();
   line.str(tmp);
   return line;
}

list<Command> read_commands(istream &in) {
   list<Command> commands;
   stringstream line;
   size_t N;
   get_next_line(line, in);
   line >> N;
   for (int n = 0; n < N; n++) {
      get_next_line(line, in);
      Command command;
      line >> command.drone;
      line >> command.type;
      if (command.type == Command::LOAD || command.type == Command::UNLOAD) 
         line >> command.warehouse;
      else if (command.type == Command::DELIVER)
         line >> command.order;
      if (command.type != Command::WAIT)
         line >> command.product;
      line >> command.value;
      assert(line);
      commands.push_back(command);
   }
   return commands;
}

}
}
}

int main(int argc, char *argv[]) {
   if (argc == 1) {
      cerr << "Usage: " << argv[0] << " <problem file>\n";
      return -1;
   }
   ifstream input(argv[1]);
   if (!input) {
      cerr << "Could not open input file '" << argv[1] << "'\n";
      return -1;
   }
   cout << "Reading input file '" << argv[1] << "' ... ";
   hashcode::Problem problem(input);
   cout << "done\n";
   problem.print_parameter(cout);
   list<Command> commands = hashcode::read_commands(cin);
   cout << "Score: " << hashcode::simulate(commands, problem) << endl;
   return 0;
}
