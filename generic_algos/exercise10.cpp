// Read a text file. Collect words in a vector.
// Convert all to lower case. Sort them. remove duplicates

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>
#include "generic_algos.h"

//**************************************************************************************************
// usage
//**************************************************************************************************
static void usage() {
  std::cout << "Usage: ./exercise10 <path to .txt file>" << std::endl;
}

//**************************************************************************************************
// GetFile (open input file)
//**************************************************************************************************
static GenericAlgoError GetFile(const int argc, char **argv,
                                std::ifstream &file) {
  if (argc < 2 || !argv) {
    return kGenericAlgoErrorBadArgs;
  }

  file.open(argv[1]);
  if (!file.is_open()) {
    return kGenericAlgoErrorUnhandled;
  }

  return kGenericAlgoErrorSuccess;
}

//**************************************************************************************************
// Function to transform string to lower case
//**************************************************************************************************
static std::string &StrToLower(std::string &s) {
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s;
}

//**************************************************************************************************
// main
//**************************************************************************************************
int main(int argc, char **argv) {

  GenericAlgoError ret;
  std::string s;
  std::ifstream file;
  std::vector<std::string> words;
  std::vector<std::string>::iterator last;

  ret = GetFile(argc, argv, file);
  if (ret != kGenericAlgoErrorSuccess) {
    usage();
    return -1;
  }

  while (file >> s) {
    words.push_back(s);
  }

  std::transform(words.begin(), words.end(), words.begin(),
                 std::ptr_fun(StrToLower));

  std::sort(words.begin(), words.end());

  last = std::unique(words.begin(), words.end());

  std::copy(words.begin(), last,
            std::ostream_iterator<std::string>(std::cout, "\n"));

  return 0;
}
