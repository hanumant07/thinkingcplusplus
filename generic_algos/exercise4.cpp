// Thinking in C++ generic algorithms exercise 4
// Word Anagram generator

#include <algorithm>
#include <iostream>

int main(int argc, char **argv) {

  std::string s;

  std::cout << "Enter word" << std::endl;

  std::cin >> s;

  std::sort(s.begin(), s.end());

  while (std::next_permutation(s.begin(), s.end())) {
    std::cout << s << std::endl;
  }

  return 0;
}
