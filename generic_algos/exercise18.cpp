
#include "generic_algos.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

//**************************************************************************************************
// Types
//**************************************************************************************************

namespace generic_algos {

// Transform input sequence elements that satisfy predicate and save to
// destination
template <typename ForwardIter, typename OutputIter, typename UnaryPred>
OutputIter transform_if(ForwardIter begin, ForwardIter last, OutputIter dest,
                        UnaryPred f) {
  while (begin != last) {
    if (f(*begin)) {
      *dest++ = *begin;
    }
    ++begin;
  }
  return dest;
}
};

// predicate to be used for transformation
template <class T> class GreaterThan : public std::unary_function<bool, T> {
  T comparater;

public:
  GreaterThan(T a) { comparater = a; }
  bool operator()(T &in) { return (in > comparater); }
};

//**************************************************************************************************
// main
//**************************************************************************************************
int main(int argc, char **argv) {

  std::vector<int> test(3, 10);
  std::vector<int>::iterator it;
  GreaterThan<int> gt(10);

  test.push_back(11);
  test.push_back(12);
  test.push_back(13);

  it = generic_algos::transform_if(test.begin(), test.end(), test.begin(), gt);

  std::copy(test.begin(), it, std::ostream_iterator<int>(std::cout, " "));

  return 0;
}
