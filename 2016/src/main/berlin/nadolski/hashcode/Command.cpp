#include "Command.h"

namespace berlin {
namespace nadolski {
namespace hashcode {

Command::Command()
: type(INVALID), warehouse(-1), drone(-1), order(-1), product(-1), value(-1)
{

}

Command::~Command() {
   // TODO Auto-generated destructor stub
}

std::ostream &operator<<(std::ostream &out, const Command &cmd) {
   out << cmd.drone << " " << (char)cmd.type << " "
         << cmd.order << " " << cmd.product << " " << cmd.value;
}

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */
