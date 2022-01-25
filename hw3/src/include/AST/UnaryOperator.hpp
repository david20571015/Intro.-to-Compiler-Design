#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class UnaryOperatorNode : public ExpressionNode {
 public:
  UnaryOperatorNode(const uint32_t line, const uint32_t col, Operator *op,
                    ExpressionNode *expr);
  ~UnaryOperatorNode();

  const char *getOperatorCString() const;
  void accept(AstNodeVisitor &p_visitor) override;
  void visitChildNodes(AstNodeVisitor &p_visitor) override;

 private:
  Operator *op;
  ExpressionNode *expr;
};

#endif
