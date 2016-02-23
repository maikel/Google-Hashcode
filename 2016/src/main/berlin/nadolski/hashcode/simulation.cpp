#include <fstream>
#include <list>
#include <cassert>
#include <sstream>

#include "Command.h"
#include "Simulation.h"

using namespace std;
using namespace berlin::nadolski;

namespace berlin {
namespace nadolski {
namespace hashcode {

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
   hashcode::Simulation simulation(input);
   cout << "done\n";
   simulation.problem.print_parameter(cout);
   list<hashcode::Command> commands = hashcode::read_commands(cin);
   cout << "Score: " << simulation.run(commands) << endl;
   return 0;
}
