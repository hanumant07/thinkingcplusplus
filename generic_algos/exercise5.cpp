// Permutations of a sentences, changing only relative positions of the words

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

void GetSortedWords(std::string &input, std::vector<std::string> &out_vec) {
  std::istringstream iss(input);
  std::string s;

  while (iss >> s) {
    out_vec.push_back(s);
  }

  std::sort(out_vec.begin(), out_vec.end());
}

int main(int argc, char **argv) {

  std::vector<std::string> words;
  std::string cmd_line;

  std::getline(std::cin, cmd_line);

  GetSortedWords(cmd_line, words);

  while (std::next_permutation(words.begin(), words.end())) {
    std::copy(words.begin(), words.end(),
              std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << std::endl;
  }

  return 0;
}
