#include "generic_algos.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_map>

typedef std::set<std::string> Dictionary;
typedef std::unordered_map<std::string, std::string> Cipher;

//**************************************************************************************************
// Get the charectars to use for encoding
//**************************************************************************************************
GenericAlgoError GetCodingChars(const std::string &coding_file,
                                std::string &coding_chars) {
  std::ifstream in(coding_file);

  if (in.fail()) {
    return kGenericAlgoErrorBadArgs;
  }
  getline(in, coding_chars);
  return kGenericAlgoErrorSuccess;
}

//**************************************************************************************************
// Get unique words from dictionary
//**************************************************************************************************
GenericAlgoError GetDictionaryWords(const std::string &dictionary_input,
                                    Dictionary &dictionary_words) {
  std::ifstream in(dictionary_input);

  if (in.fail()) {
    return kGenericAlgoErrorBadArgs;
  }

  std::string line;
  while (std::getline(in, line)) {
    std::stringstream iss(line);

    while (!iss.eof()) {
      std::string word;

      iss >> word;
      dictionary_words.insert(word);
    }
  }
  return kGenericAlgoErrorSuccess;
}

//**************************************************************************************************
// Emit the coded dictionary
//**************************************************************************************************
GenericAlgoError WriteCodedDictionary(Cipher &cipher,
                                      const std::string &dictionary,
                                      const std::string &coded_dictionary) {

  std::ifstream in(dictionary);
  std::ofstream out(coded_dictionary);

  if (in.fail() || out.fail()) {
    return kGenericAlgoErrorBadArgs;
  }

  std::string line;
  while (std::getline(in, line)) {
    std::stringstream iss(line);

    while (!iss.eof()) {
      std::string word;

      iss >> word;
      if (cipher.find(word) == cipher.end()) {
        return kGenericAlgoErrorBadArgs;
      }

      out << cipher[word] << " ";
    }
    out << "\n";
  }
  return kGenericAlgoErrorSuccess;
}

int main(int argc, char **argv) {

  GenericAlgoError ret;
  Dictionary dictionary_words;
  Cipher cipher;
  std::string coding_chars;

  ret = GetCodingChars("coding_chars.txt", coding_chars);
  if (ret != kGenericAlgoErrorSuccess) {
    return ret;
  }

  ret = GetDictionaryWords("dictionary.txt", dictionary_words);
  if (ret != kGenericAlgoErrorSuccess) {
    return ret;
  }

  sort(coding_chars.begin(), coding_chars.end());
  auto it = dictionary_words.begin();

  while (it != dictionary_words.end()) {

    if (!next_permutation(coding_chars.begin(), coding_chars.end())) {
      std::cout << "Insufficient coding charetars to encode dictionary"
                << std::endl;
    }

    cipher[*it] = coding_chars;
    ++it;
  }

  ret = WriteCodedDictionary(cipher, "dictionary.txt", "coded_dictionary.txt");
  if (ret != kGenericAlgoErrorSuccess) {
    std::cout << "Unable to write coded dictionary ret =  " << ret << std::endl;
  }

  return 0;
}
