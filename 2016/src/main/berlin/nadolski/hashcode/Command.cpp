#include "Command.h"

namespace berlin {
namespace nadolski {
namespace hashcode {

Command::Command()
: type(INVALID), warehouse(-1), drone(-1), order(-1), product(-1), value(-1)
{

}

Command::Command(int drone, Type t, int dest, int product, int value)
: Command() {
   this->drone = drone;
   this->type  = t;
   if (this->type == DELIVER)
      this->order = dest;
   else
      this->warehouse = dest;
   this->product = product;
   this->value = value;
}

Command::~Command() {
   // TODO Auto-generated destructor stub
}

std::ostream &operator<<(std::ostream &out, const Command &cmd) {
   out << cmd.drone << " " << (char)cmd.type << " ";
   if (cmd.type == Command::LOAD || cmd.type == Command::UNLOAD)
      out << cmd.warehouse << " " << cmd.product;
   else if (cmd.type == Command::DELIVER)
      out << cmd.order << " " << cmd.product;
   out << " " << cmd.value;
}

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */
