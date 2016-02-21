#include <iostream>
#include <string>
#include <sstream>
#include "Problem.h"

namespace berlin {
namespace nadolski {
namespace hashcode {

std::stringstream &get_next_line(std::stringstream &line, std::istream &in) {
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
Problem::Problem(std::istream &in) {
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
   products.resize(N);
   get_next_line(line, in);
   for (size_t n = 0; n < N; n++)
      line >> products[n];
   if (!line) {
      std::cerr << "Could not read all product informations.\n";
      std::exit(-1);
   }

   // read warehouse informations
   get_next_line(line, in);
   line >> N;
   warehouses.resize(N);
   for (size_t n = 0; n < N; n++) {
      get_next_line(line, in);
      line >> warehouses[n].x;
      line >> warehouses[n].y;
      warehouses[n].products.resize(products.size());
      get_next_line(line, in);
      for (size_t k = 0; k < warehouses[n].products.size(); k++) {
         line >> warehouses[n].products[k];
      }
      if (!line) {
         std::cerr << "Could not read warehouse informations.\n";
         std::cerr << "Warehouse Number: " << n << std::endl;
         exit(-1);
      }
   }

   // read orders
   get_next_line(line, in);
   line >> N;
   orders.resize(N);
   for (size_t n = 0; n < N; n++) {
      get_next_line(line, in);
      line >> orders[n].x;
      line >> orders[n].y;
      get_next_line(line, in);
      int K;
      line >> K;
      orders[n].products.resize(K);
      get_next_line(line, in);
      for (size_t k = 0; k < orders[n].products.size(); k++)
         line >> orders[n].products[k];
      if (!line) {
         std::cerr << "Could not read order informations.\n";
         std::cerr << "Order Number: " << n << std::endl;
         exit(-1);
      }
   }
}

Problem::~Problem() {
   // TODO Auto-generated destructor stub
}

/**
 * Print parameters for general information.
 */
void Problem::print_parameter(std::ostream &out) {
   out << "Map Dimension: (" << rows << ", " << cols << ")\n";
   out << "Number of Drones: " << drones.size() << std::endl;
   out << "Maximum Load Capacity of Drones: " << max_load << std::endl;
   out << "Max Steps of Simulation: " << deadline << std::endl;
   out << "Number of Prodcuts: " << products.size() << std::endl;
   out << "Number of Warehouses: " << warehouses.size() << std::endl;
   out << "Number of Orders: " << orders.size() << std::endl;
}


} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */
