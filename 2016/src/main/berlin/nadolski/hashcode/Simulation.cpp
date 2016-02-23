#include "Simulation.h"

#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;

namespace berlin {
namespace nadolski {
namespace hashcode {

static int perform_deliver(Drone &drone, int t, int T, Problem &problem)
{
   assert(t < T);
   assert(drone.busy == 1);
   const Command &command = drone.command;
   assert(problem.orders.size() > command.order);
   vector<int> &products = problem.orders[command.order].products;
   assert(products.size() > command.product);
   products[command.product] -= command.value;
   assert(products[command.product] >= 0);
   drone.x = problem.orders[command.order].x;
   drone.y = problem.orders[command.order].y;

   // check if we completed the order
   bool completed = true;
   for (int i = 0; i < products.size(); i++)
      if (products[i] > 0)
         completed = false;
   // calculate a score if we finished the order
   int score = 0;
   if (completed) {
      assert(ceil(float((T - t) * 100) / T) > 0);
      score += ceil(float((T - t) * 100) / T);
      cout << "Completing order " << command.order << " and scoring " << score
            << " Points.\n";
   }
   return score;
}

static void perform_load(Drone &drone, Problem &problem)
{
   assert(drone.busy == 1);
   Command &command = drone.command;
   assert(problem.warehouses.size() > command.warehouse);
   vector<int> &products = problem.warehouses[command.warehouse].products;
   assert(products.size() > command.product);
   assert(products[command.product] >= command.value);
   assert(
         problem.max_load >= problem.products[command.product] * command.value);
   products[command.product] -= command.value;
   // set new coordinates for drone
   drone.x = problem.warehouses[command.warehouse].x;
   drone.y = problem.warehouses[command.warehouse].y;
}

static void perform_unload(Drone &drone, Problem &problem)
{
   assert(drone.busy == 1);
   Command &command = drone.command;
   assert(problem.warehouses.size() > command.warehouse);
   vector<int> &products = problem.warehouses[command.warehouse].products;
   assert(products.size() > command.product);
   products[command.product] += command.value;
   // set new coordinates for drone
   drone.x = problem.warehouses[command.warehouse].x;
   drone.y = problem.warehouses[command.warehouse].y;
}

static int perform_commands(int t, int T, Problem &problem)
{
   int score = 0;
   // perform pending commands and increase score if neccessary
   for (int d = 0; d < problem.drones.size(); d++) {
      Drone &drone = problem.drones[d];
      if (drone.busy == 1) {
         cout << "Drone " << d << " performs its assinged command: "
               << drone.command << endl;
         switch (drone.command.type) {
         case Command::deliver: {
            score = perform_deliver(drone, t, T, problem);
            break;
         }
         case Command::load: {
            perform_load(drone, problem);
            break;
         }
         case Command::unload: {
            perform_unload(drone, problem);
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

float distance(int x1, int y1, int x2, int y2)
{
   return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

static list<Command> assign_commands(list<Command> &commands, Problem &p)
{
   list<Command> queue = commands;
   commands = list<Command>();
   while (queue.size() > 0) {
      Command cmd = queue.front();
      queue.pop_front();
      assert(p.drones.size() > cmd.drone);
      if (p.drones[cmd.drone].busy == 0) {
         cout << "Drone " << cmd.drone << " recieves a new command " << cmd
               << endl;
         assert(p.drones.size() > cmd.drone);
         Drone &drone = p.drones[cmd.drone];
         drone.command = cmd;
         switch (cmd.type) {
         case Command::wait:
            drone.busy = cmd.value;
            break;
         case Command::load:
            assert(p.warehouses.size() > cmd.warehouse);
            drone.busy = ceil(
                  distance(p.warehouses[cmd.warehouse].x,
                        p.warehouses[cmd.warehouse].y, drone.x, drone.y)) + 1;
            break;
         case Command::deliver:
            assert(p.orders.size() > cmd.order);
            drone.busy = ceil(
                  distance(p.orders[cmd.order].x, p.orders[cmd.order].y,
                        drone.x, drone.y)) + 1;
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
      problem(in)
{

}

int Simulation::run(list<Command> commands)
{
   int score = 0;
   int T = problem.deadline;

   Problem problem_of_this_run(problem);
   for (int t = 0; t < T; t++) {
      cout << "time step t = " << t << endl;
      commands = assign_commands(commands, problem_of_this_run);
      score += perform_commands(t, T, problem_of_this_run);
   }
   return score;
}

Simulation::~Simulation()
{

}

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */
