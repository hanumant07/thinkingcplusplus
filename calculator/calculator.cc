
#pragma once
#include <sstream>

//**************************************************************************************************
// Add an operator to the calculator vocabulary
//**************************************************************************************************
template <typename T>
CalculatorError Calculator<T>::AddOperator(char c, T (*fn)(T, T),
                                           uint32_t priority) {

  if (this->operator_map.find(c) != this->operator_map.end()) {
    std::cout << "Operator symbol already exists";
    return kCalculatorErrorBadArgs;
  }

  Operator<T> *op = new Operator<T>(c, fn, priority);
  if (op == nullptr) {
    return kCalculatorErrorNoMem;
  }

  this->operator_map.insert(std::make_pair(c, op));
  std::cout << "Added operator " << *op << std::endl;
  return kCalculatorErrorSuccess;
}

//**************************************************************************************************
// Handle Error
//**************************************************************************************************
template <typename T> T Calculator<T>::HandleError(CalculatorError e) {
  std::cout << "Error e " << e << std::endl;

  // Empty all the stacks
  while (!this->operator_stack.empty())
    this->operator_stack.pop();

  while (!this->operand_stack.empty())
    this->operand_stack.pop();

  // For all error scenarios display a 0 as calculator output
  return T(0);
}

//**************************************************************************************************
// Pop 2 operands and the top operator and compute the result.
// Push result back , as an operand
//**************************************************************************************************
template <typename T> CalculatorError Calculator<T>::PopAndCompute() {
  // If operand stack is empty this is an invalid expression
  if (this->operand_stack.empty()) {
    return kCalculatorErrorExprInvalid;
  }

  Operand<T> *op1 = this->operand_stack.top();
  T val1 = op1->GetValue();
  this->operand_stack.pop();

  // If we cannot extrace second operand, the expression is invalid
  if (this->operand_stack.empty()) {
    return kCalculatorErrorExprInvalid;
  }

  Operand<T> *op2 = this->operand_stack.top();
  T val2 = op2->GetValue();
  this->operand_stack.pop();

  // If operator stack is empty this is an invalid expression
  if (this->operator_stack.empty()) {
    return kCalculatorErrorExprInvalid;
  }

  Operator<T> *op = this->operator_stack.top();
  T result = (*op)(val1, val2);
  std::cout << result << " = " << val1 << " " << *op << " " << val2
            << std::endl;
  this->operator_stack.pop();

  Operand<T> *result_opr = new Operand<T>(result);
  if (result_opr == nullptr) {
    return kCalculatorErrorNoMem;
  }

  this->operand_stack.push(result_opr);
  return kCalculatorErrorSuccess;
}

//**************************************************************************************************
// Handle Operand occurence
//**************************************************************************************************
template <typename T>
CalculatorError Calculator<T>::HandleOperand(Operand<T> *opr) {

  if (opr == nullptr) {
    return kCalculatorErrorBadArgs;
  }

  if (this->last_token_type == kCalculatorOperand) {
    return kCalculatorErrorExprInvalid;
  }

  this->last_token_type = kCalculatorOperand;

  this->operand_stack.push(opr);

  return kCalculatorErrorSuccess;
}

//**************************************************************************************************
// Handle Operator occurence
//**************************************************************************************************
template <typename T>
CalculatorError Calculator<T>::HandleOperator(Operator<T> *curr_op) {

  if (!curr_op) {
    return kCalculatorErrorBadArgs;
  }

  // This represents the case of first token being an operator, which is invalid
  if (this->last_token_type == kCalculatorNone) {
    return kCalculatorErrorExprInvalid;
  }

  // This represents the case of 2 back to back operators
  if (this->last_token_type == kCalculatorOperator) {
    return kCalculatorErrorExprInvalid;
  }

  this->last_token_type = kCalculatorOperator;
  // If this is first operator push it
  if (this->operator_stack.empty()) {
    this->operator_stack.push(curr_op);
    return kCalculatorErrorSuccess;
  }

  Operator<T> *stack_top = this->operator_stack.top();
  // if the current top of stack operator has lower precedence then the new
  // operator
  // push new operator on stack
  if (*stack_top < *curr_op) {
    this->operator_stack.push(curr_op);
    return kCalculatorErrorSuccess;
  }
  // The top of operator stack has higher precedence. Perform related
  // computation
  return this->PopAndCompute();
}

//**************************************************************************************************
// get operator from string. Return NULL if no valid operator found
//**************************************************************************************************
template <typename T>
Operator<T> *Calculator<T>::GetOperator(const std::string &input) {

  auto it = this->operator_map.begin();
  for (; it != this->operator_map.end(); ++it) {
    if (it->second->IsOperator(input)) {
      return new Operator<T>(*(it->second));
    }
  }
  return nullptr;
}

//**************************************************************************************************
// Get Operand
//**************************************************************************************************
template <typename T>
Operand<T> *Calculator<T>::GetOperand(const std::string &input) {
  return Operand<T>::GetOperand(input);
}
//**************************************************************************************************
// Parse a given string expression to extract operators and operands.
// If invalid , return 0
//**************************************************************************************************
template <typename T>
T Calculator<T>::ParseAndCompute(const std::string &expr) {

  if (!expr.length()) {
    return this->HandleError(kCalculatorErrorExprInvalid);
  }

  CalculatorError ret;
  std::stringstream iss(expr);
  std::string token;

  while (!iss.eof()) {
    Operand<T> *opr;
    Operator<T> *op;

    iss >> token;
    if ((op = this->GetOperator(token)) != nullptr) {

      ret = this->HandleOperator(op);

      if (ret != kCalculatorErrorSuccess) {
        return this->HandleError(ret);
      }
      continue;
    } else if ((opr = this->GetOperand(token)) != nullptr) {

      ret = this->HandleOperand(opr);

      if (ret != kCalculatorErrorSuccess) {
        return this->HandleError(ret);
      }
      continue;
    } else {
      return this->HandleError(kCalculatorErrorExprInvalid);
    }
  }
  // Encountered the end of expression
  // Process the pending operators and operands on the stack.
  // The result will be left in the operand stack
  while (!this->operator_stack.empty()) {
    ret = this->PopAndCompute();

    if (ret != kCalculatorErrorSuccess) {
      return this->HandleError(ret);
    }
  }

  T result = this->operand_stack.top()->GetValue();
  this->operand_stack.pop();
  return result;
}

//**************************************************************************************************
// Parse a string to get an operand object
//**************************************************************************************************
template <>
Operand<double> *Operand<double>::GetOperand(const std::string &input) {
  double val;
  try {
    val = std::stod(input);
  } catch (...) {
    return nullptr;
  }
  return new Operand<double>(val);
}

//**************************************************************************************************
// Check if input string is an operator symbol
//**************************************************************************************************
template <typename T> bool Operator<T>::IsOperator(const std::string &input) {
  if (input.length() > 1 || input.length() == 0) {
    return false;
  }
  return (input[0] == this->GetSymbol());
}
