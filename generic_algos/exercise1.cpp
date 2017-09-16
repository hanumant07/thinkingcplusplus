
#include <algorithm>
#include <ctime>
#include <iostream>
#include <list>

class ClockGen {
  clock_t val;

public:
  clock_t operator()() {
    val = std::clock();
    return val;
  };
};

int main(int argc, char **argv) {
  std::list<clock_t> clock_list(15);
  ClockGen clock_gen;

  // std::cout << clock();
  std::generate_n(clock_list.begin(), 15, clock_gen);

  std::unique(clock_list.begin(), clock_list.end());

  std::copy(clock_list.begin(), clock_list.end(),
            std::ostream_iterator<clock_t>(std::cout, " "));

  return 0;
}
