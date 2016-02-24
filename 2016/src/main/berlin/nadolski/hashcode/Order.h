#ifndef ORDER_H_
#define ORDER_H_

#include <vector>

namespace berlin {
namespace nadolski {
namespace hashcode {

struct Order {
   int id;
   int x;
   int y;
   std::vector<int> products;

   bool operator==(Order &other)
   {
      return id == other.id;
   }
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* ORDER_H_ */
