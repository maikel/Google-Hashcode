#ifndef BERLIN_NADOLSKI_HASHCODE_MYSTRATEGY_H_
#define BERLIN_NADOLSKI_HASHCODE_MYSTRATEGY_H_

#include "ProblemState.h"
#include "Strategy.h"

#include <vector>

namespace berlin {
namespace nadolski {
namespace hashcode {

class MyStrategy {
public:
   MyStrategy(std::istream &in);
   MyStrategy(const ProblemState &state);
   virtual ~MyStrategy() {}

   std::vector<Command> generate_commands();
private:
   ProblemState state;
};

}
}
}


#endif /* BERLIN_NADOLSKI_HASHCODE_MYSTRATEGY_H_ */
