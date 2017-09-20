#pragma once
#include <iostream>
#include <map>
#include <stack>

typedef enum {
  kCalculatorErrorSuccess = 0,
  kCalculatorErrorBadArgs = -1,
  kCalculatorErrorNoMem = -2,
  kCalculatorErrorExprInvalid = -3,
} CalculatorError;

template <typename T> class Calculator;

template <typename U> class Operator {
public:
  // Compare Operator precedence
  bool operator<(const Operator<U> &op) {
    return this->priority < op.priority;
  };
  // Print Operator
  friend std::ostream &operator<<(std::ostream &os, Operator<U> &op) {
    return os << op.symbol;
  };

private:
  Operator(char c, U (*c_fn)(U, U), uint32_t p)
      : symbol(c), priority(p), fn(c_fn){};
  // Make Default constructor unavailable
  Operator<U>(){};
  // Make Assignment operatorion invalid
  Operator<U> &operator=(const Operator<U> &) {}
  // Check if a given string is an operator
  bool IsOperator(const std::string &);
  // Get the symbol
  char GetSymbol() { return this->symbol; }
  U operator()(U a, U b) { return (*fn)(a, b); }
  // Only Calculator can create objects
  friend class Calculator<U>;
  // Data members
  // The computation function associated with the operator
  U (*fn)(U, U);
  // symbol for the operator
  char symbol;
  // precedence
  uint32_t priority;
};

template <typename U> class Operand {
public:
  friend std::ostream &operator<<(std::ostream &os, Operand<U> &op) {
    return os << op.val;
  }
  static Operand<U> *GetOperand(const std::string &);

private:
  Operand(U v) : val(v) {}
  Operand() {}
  Operand operator=(const Operand &) {}
  U GetValue() { return this->val; }
  typedef U OperandType;
  OperandType val;
  friend class Calculator<U>;
};

template <typename T> class Calculator {
public:
  CalculatorError AddOperator(char c, T (*fn)(T, T), uint32_t priority);
  T ParseAndCompute(const std::string &expression);

private:
  typedef enum {
    kCalculatorNone = 0,
    kCalculatorOperand,
    kCalculatorOperator,
  } CalculatorTokenType;
  Operator<T> *GetOperator(const std::string &);
  Operand<T> *GetOperand(const std::string &);
  T HandleError(CalculatorError e);
  CalculatorError HandleOperator(Operator<T> *op);
  CalculatorError HandleOperand(Operand<T> *opr);
  CalculatorError PopAndCompute();
  CalculatorTokenType last_token_type;
  // Stack of operands
  std::stack<Operand<T> *> operand_stack;
  // Stack of operators
  std::stack<Operator<T> *> operator_stack;
  // All operators mapped by symbol
  std::map<char, Operator<T> *> operator_map;
};

#include "calculator.cc"
