#include <iostream>
#include <fstream>
#include <vector>

#include "MyStrategy.h"

using namespace std;
using namespace berlin::nadolski;

int main(int argc, char *argv[])
{
   if (argc == 1) {
      cerr << "Usage: " << argv[0] << " <problem file>\n";
      return -1;
   }
   ifstream input(argv[1]);
   if (!input) {
      cerr << "Could not open input file '" << argv[1] << "'\n";
      return -1;
   }
   hashcode::MyStrategy strat(input);
   vector<hashcode::Command> commands = strat.generate_commands();
   cout << commands.size() << endl;
   for (auto command : commands)
      cout << command << "\n";
   cout << flush;
   return 0;
}
