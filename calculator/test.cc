#include "calculator.h"
#include <functional>
#include <iostream>

template <typename T> T Multiply(T a, T b) { return a * b; }

template <typename T> T Min(T a, T b) { return a < b ? a : b; }

template <typename T> T Max(T a, T b) { return a > b ? a : b; }

template <typename T> T Plus(T a, T b) { return a + b; }

int main(int argc, char **argv) {
  Calculator<double> calc;

  calc.AddOperator('+', &Plus<double>, 0);
  calc.AddOperator('*', &Multiply<double>, 1);
  calc.AddOperator('&', &Max<double>, 2);
  calc.AddOperator('|', &Min<double>, 2);
  Operand<double> *op = Operand<double>::GetOperand("12.34");
  std::cout << *op << std::endl;
  calc.ParseAndCompute("2 + 3");
  std::cout << "Done " << std::endl;
  return 0;
}
