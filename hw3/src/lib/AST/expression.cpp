#include "AST/expression.hpp"

Operator::Operator(OperatorType type) : type(type) {}

const char* Operator::getTypeCString() {
  if (type_str.empty()) {
    setTypeStr();
  }
  return type_str.c_str();
}

void Operator::setTypeStr() {
  switch (type) {
    case OperatorType::PLUS_OP:
      type_str = "+";
      break;
    case OperatorType::MINUS_OP:
      type_str = "-";
      break;
    case OperatorType::MULTIPLY_OP:
      type_str = "*";
      break;
    case OperatorType::DIVIDE_OP:
      type_str = "/";
      break;
    case OperatorType::MOD_OP:
      type_str = "mod";
      break;
    case OperatorType::EQUAL_OP:
      type_str = "=";
      break;
    case OperatorType::NOT_EQUAL_OP:
      type_str = "<>";
      break;
    case OperatorType::LESS_OP:
      type_str = "<";
      break;
    case OperatorType::LESS_OR_EQUAL_OP:
      type_str = "<=";
      break;
    case OperatorType::GREATER_OP:
      type_str = ">";
      break;
    case OperatorType::GREATER_OR_EQUAL_OP:
      type_str = ">=";
      break;
    case OperatorType::AND_OP:
      type_str = "and";
      break;
    case OperatorType::OR_OP:
      type_str = "or";
      break;
    case OperatorType::NOT_OP:
      type_str = "not";
      break;
    case OperatorType::UNARY_MINUS_OP:
      type_str = "neg";
      break;
    case OperatorType::NONE:
      type_str = "none";
      break;
  }
}

ExpressionNode::ExpressionNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
