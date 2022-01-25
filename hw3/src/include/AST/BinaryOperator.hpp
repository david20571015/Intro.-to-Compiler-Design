#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include <memory>

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class BinaryOperatorNode : public ExpressionNode {
 public:
  BinaryOperatorNode(const uint32_t line, const uint32_t col,
                     ExpressionNode *left, Operator *op, ExpressionNode *right);
  ~BinaryOperatorNode();

  const char *getOperatorCString() const;
  void accept(AstNodeVisitor &p_visitor) override;
  void visitChildNodes(AstNodeVisitor &p_visitor) override;

 private:
  ExpressionNode *left, *right;
  Operator *op;
};

#endif
