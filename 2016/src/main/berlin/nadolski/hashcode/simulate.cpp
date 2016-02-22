#include <fstream>
#include <memory>
#include <list>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <sstream>
#include <string>
#include <queue>

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

int simulate_perform_commands(int t, int T, Problem &p) {
   int score = 0;
   // perform pending commands and increase score if neccessary
   for (Drone &drone : p.drones) {
      if (drone.busy == 1) {
         const Command &cmd = drone.command;
         switch (cmd.type) {
         case Command::DELIVER:
         {
            vector<int> &products = p.orders[cmd.order].products;
            assert(products.size() > cmd.product);
            products[cmd.product] -= cmd.value;
            assert(products[cmd.product] >= 0);
            // check if we completed the order
            bool completed = true;
            for (int i = 0; i < products.size(); i++)
               if (products[i] > 0)
                  completed = false;
            if (completed) {
               assert(ceil(float((T - t) * 100) / T) > 0);
               score += ceil(float((T - t) * 100) / T);
               cout << "Drone " << cmd.drone << " dilivered "
                    << "order " << cmd.order << " in turn " << t
                    << " for " << score << " points!\n";
            }
            break;
         }
         case Command::LOAD:
         {
            assert(p.warehouses.size() > cmd.warehouse);
            vector<int> &products = p.warehouses[cmd.warehouse].products;
            assert(products.size() > cmd.product);
            assert(products[cmd.product] >= cmd.value);
            products[cmd.product] -= cmd.value;
            break;
         }
         case Command::UNLOAD:
         {
            assert(p.warehouses.size() > cmd.warehouse);
            vector<int> &products = p.warehouses[cmd.warehouse].products;
            assert(products.size() > cmd.product);
            products[cmd.product] += cmd.value;
            break;
         }
         default:
            break;
         }
      }
      cout << "warehouses[0] products = [";
      for (int ip : p.warehouses[0].products)
         cout << ip << " ";
      cout << "]\n";
      cout << "warehouses[1] products = [";
      for (int ip : p.warehouses[1].products)
         cout << ip << " ";
      cout << "]\n";
      cout << "order[0] products = [";
      for (int ip : p.orders[0].products)
         cout << ip << " ";
      cout << "]\n";
      cout << "order[1] products = [";
      for (int ip : p.orders[1].products)
         cout << ip << " ";
      cout << "]\n";
      cout << "order[2] products = [";
      for (int ip : p.orders[2].products)
         cout << ip << " ";
      cout << "]\n";
      if (drone.busy > 0) {
         cout << "Drone " << drone.command.drone << " waits or flies.\n";
         drone.busy -= 1;
      }
   }
   return score;
}

float distance(int x1, int y1, int x2, int y2) {
   return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

list<Command> simulate_apply_commands(list<Command> &commands, Problem &p) {
   list<Command> queue = commands;
   commands = list<Command>();
   while (queue.size() > 0) {
      Command cmd = queue.front();
      queue.pop_front();
      assert(p.drones.size() > cmd.drone);
      if (p.drones[cmd.drone].busy == 0) {
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
            cout << cmd.warehouse << " " << p.warehouses[cmd.warehouse].x << " " <<
               p.warehouses[cmd.warehouse].y << " " << 
               drone.x << " " << drone.y << endl;
            cout << "distance " << distance(
               p.warehouses[cmd.warehouse].x,
               p.warehouses[cmd.warehouse].y,
               drone.x, drone.y) << endl;
            cout << "Drone " << cmd.drone << " is busy for " << drone.busy << " turns.\n";
            break;
         case Command::DELIVER:
            assert(p.orders.size() > cmd.order);
            drone.busy = ceil(distance(
               p.orders[cmd.order].x, p.orders[cmd.order].y,
               drone.x, drone.y)) + 1;
            cout << "Drone " << cmd.drone << " is busy for " << drone.busy << " turns.\n";
            break;
         default:
            assert(0);
            break;
         }
         // cout << "Drone " << cmd.drone << " is busy for " << drone.busy << " turns.\n";
      } else {
         commands.push_back(cmd);
      }
   }
   return commands;
}

/**
 * Simulate the problem and calculate the total score of a given solution.
 */
int simulate(list<Command> commands, Problem p) {
   int score = 0;
   int T = p.deadline;

   for (int t = 0; t < T; t++) {
      // cout << "time step t = " << t << endl;
      // apply to each drone a command and remove given commands from list
      commands = simulate_apply_commands(commands, p);
      score += simulate_perform_commands(t, T, p);
   }
   return score;
}

static std::stringstream &get_next_line(stringstream &line, istream &in) {
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
   assert(line);
   for (int n = 0; n < N; n++) {
      get_next_line(line, in);
      Command command;
      char type;
      line >> command.drone;
      line >> type;
      command.type = (Command::Type)type;
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
   list<hashcode::Command> commands = hashcode::read_commands(cin);
   cout << "Score: " << hashcode::simulate(commands, problem) << endl;
   return 0;
}
