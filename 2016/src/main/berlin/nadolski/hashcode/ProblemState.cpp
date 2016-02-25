#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include "ProblemState.h"

namespace berlin {
namespace nadolski {
namespace hashcode {

static std::stringstream &get_next_line(std::stringstream &line, std::istream &in) {
   if (!in) {
      std::cerr << "Some input error occured.\n";
      std::exit(-1);
   }
   std::string tmp;
   std::getline(in, tmp);
   line.clear();
   line.str(tmp);
   return line;
}

/**
 * Read input file which is defined in the Hashcode problem set.
 */
ProblemState::ProblemState(std::istream &in) {
   size_t N;
   std::stringstream line;

   get_next_line(line, in);
   // read simulation parameters
   line >> rows;
   line >> cols;
   line >>  N;
   line >> deadline;
   line >> max_load;
   drones.resize(N);
   if (!line) {
      std::cerr << "Could not read global parameters.\n";
      std::exit(-1);
   }

   // read product informations
   get_next_line(line, in);
   line >> N;
   product_weights.resize(N);
   get_next_line(line, in);
   for (size_t n = 0; n < N; n++)
      line >> product_weights[n];
   if (!line) {
      std::cerr << "Could not read all product informations.\n";
      std::exit(-1);
   }

   // read warehouse informations
   get_next_line(line, in);
   line >> N;
   assert(line);
   warehouses.resize(N);
   for (size_t n = 0; n < N; n++) {
      warehouses[n].id = n;
      get_next_line(line, in);
      line >> warehouses[n].x;
      line >> warehouses[n].y;
      assert(line);
      warehouses[n].products.resize(product_weights.size());
      get_next_line(line, in);
      for (size_t k = 0; k < warehouses[n].products.size(); k++) {
         line >> warehouses[n].products[k];
      }
      assert(line);
   }

   // read orders
   get_next_line(line, in);
   line >> N;
   orders.resize(N);
   for (size_t n = 0; n < N; n++) {
      orders[n].id = n;
      get_next_line(line, in);
      line >> orders[n].x;
      line >> orders[n].y;
      get_next_line(line, in);
      int K;
      line >> K;
      orders[n].products.resize(product_weights.size());
      get_next_line(line, in);
      for (size_t k = 0; k < K; k++) {
         int product;
         line >> product;
         assert(orders[n].products.size() > product);
         orders[n].products[product] += 1;
      }
      if (!line) {
         std::cerr << "Could not read order informations.\n";
         std::cerr << "Order Number: " << n << std::endl;
         exit(-1);
      }
   }

   // put drones to warehouse 0
   for (int id = 0; id < drones.size(); id++) {
      drones[id].id = id;
      drones[id].x = warehouses[0].x;
      drones[id].y = warehouses[0].y;
   }
}

ProblemState::~ProblemState() {
   // TODO Auto-generated destructor stub
}

/**
 * Print parameters for general information.
 */
void ProblemState::print_parameter(std::ostream &out) {
   out << "Map Dimension: (" << rows << ", " << cols << ")\n";
   out << "Number of Drones: " << drones.size() << std::endl;
   out << "Maximum Load Capacity of Drones: " << max_load << std::endl;
   out << "Max Steps of Simulation: " << deadline << std::endl;
   out << "Number of Prodcuts: " << product_weights.size() << std::endl;
   out << "Number of Warehouses: " << warehouses.size() << std::endl;
   out << "Number of Orders: " << orders.size() << std::endl;
}


} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */
