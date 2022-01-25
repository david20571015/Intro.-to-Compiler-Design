#ifndef __AST_EXPRESSION_NODE_H
#define __AST_EXPRESSION_NODE_H

#include <string>

#include "AST/ast.hpp"

enum OperatorType {
  PLUS_OP,
  MINUS_OP,
  MULTIPLY_OP,
  DIVIDE_OP,
  MOD_OP,
  EQUAL_OP,
  NOT_EQUAL_OP,
  LESS_OP,
  LESS_OR_EQUAL_OP,
  GREATER_OP,
  GREATER_OR_EQUAL_OP,
  AND_OP,
  OR_OP,
  NOT_OP,
  UNARY_MINUS_OP,
  NONE
};

class Operator {
 public:
  Operator(OperatorType type);

  const char* getTypeCString();

 private:
  OperatorType type;
  std::string type_str = "";

  void setTypeStr();
};

class ExpressionNode : public AstNode {
 public:
  ExpressionNode(const uint32_t line, const uint32_t col);
  ~ExpressionNode() = default;

 protected:
  // for carrying type of result of an expression
  // TODO: for next assignment
};

#endif
