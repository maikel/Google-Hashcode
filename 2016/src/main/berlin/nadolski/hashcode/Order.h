#ifndef ORDER_H_
#define ORDER_H_

#include <array>
#include <vector>

namespace berlin {
namespace nadolski {
namespace hashcode {

class Order {
public:
   Order();
   virtual ~Order();

   int x;
   int y;
   std::vector<int> products;

   bool operator==(const Order &o) const {
      return this == std::addressof(o);
   }
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* ORDER_H_ */
