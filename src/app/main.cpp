#include <iostream>
#include <iterator>
#include <set>
#include "trees.hpp"

const int kUpperBound = 10;

template <typename C, typename T>
long long RangeQuery(const C& container, T lhs, T rhs) {
  using Iter = typename C::const_iterator;
  Iter start = container.lower_bound(lhs);
  Iter fin = container.upper_bound(rhs);
  return std::distance(start, fin);
}


int main() {
  std::set<int> std_container {5, 3, 7};
  std::cout << RangeQuery<std::set<int>, int>(std_container, 3, 7) << "\n";
  std::cout << RangeQuery<std::set<int>, int>(std_container, 2, 6) << "\n";

  return 0;
}
