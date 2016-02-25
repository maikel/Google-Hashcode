#include <iostream>
#include <fstream>
#include <list>
#include "NearestWarehouse.h"
#include "ProblemState.h"

using namespace std;
using namespace berlin::nadolski;

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
   hashcode::NearestWarehouse nearest(input);
//    nearest.print_parameter(cout);
   list<hashcode::Command> commands = nearest.generate_commands();
   cout << commands.size() << endl;
   for (hashcode::Command command : commands)
      cout << command << endl;
   return 0;
}
