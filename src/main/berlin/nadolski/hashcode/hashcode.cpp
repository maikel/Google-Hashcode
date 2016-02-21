#include <fstream>
#include <memory>
#include <list>
#include "Problem.h"
#include "Command.h"
#include "SimpleStrategy.h"


using namespace std;
using namespace berlin::nadolski;

int main(int argc, char *argv[]) {
   if (argc == 1) {
      cerr << "Usage: " << argv[0] << " <filename>\n";
      return 0;
   }
   ifstream input(argv[1]);
   if (!input) {
      return -1;
   }
   cerr << "Reading input file '" << argv[1] << "' ... ";
   hashcode::Problem problem(input);
   cerr << "done\n";
   problem.print_parameter(cerr);

   list<shared_ptr<hashcode::Strategy>> strats;
   strats.push_back(static_pointer_cast<hashcode::Strategy>(
         make_shared<hashcode::SimpleStrategy>(problem)));

   for (auto strategy : strats) {
      cerr << "Generate commands with Strategy '" << strategy->name() << "' ... ";
      list<hashcode::Command> cmds = strategy->generate_commands();
      cerr << "done. Produced " << cmds.size() << " commands.\n";
      if (!hashcode::validate(cmds, strategy->problem)) {
         cerr << "Could not validate the command list of strategy '"
               << strategy->name() << "'.\n";
      } else {
         cout << "Score: " << hashcode::simulate(cmds, strategy->problem) << endl;
      }
   }

   return 0;
}
