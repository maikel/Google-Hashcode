#ifndef WAREHOUSE_H_
#define WAREHOUSE_H_

#include <vector>

namespace berlin {
namespace nadolski {
namespace hashcode {


class Warehouse {
public:
   Warehouse();
   virtual ~Warehouse();

   int x;
   int y;
   std::vector<int> products;
};

} /* namespace hashcode */
} /* namespace nadolski */
} /* namespace berlin */

#endif /* WAREHOUSE_H_ */
