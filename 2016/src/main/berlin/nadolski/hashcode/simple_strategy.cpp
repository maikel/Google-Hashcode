#include <iostream>
#include <fstream>
#include <list>

#include "ProblemState.h"
#include "SimpleStrategy.h"

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
   hashcode::SimpleStrategy simple(input);
   list<hashcode::Command> commands = simple.generate_commands();
   cout << commands.size() << endl;
   for (hashcode::Command command : commands)
      cout << command << endl;
   return 0;
}
