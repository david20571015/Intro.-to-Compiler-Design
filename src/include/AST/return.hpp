#ifndef __AST_RETURN_NODE_H
#define __AST_RETURN_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class ReturnNode : public AstNode {
 public:
  ReturnNode(const uint32_t line, const uint32_t col, ExpressionNode* expr);
  ~ReturnNode();

  void accept(AstNodeVisitor& p_visitor) override;
  void visitChildNodes(AstNodeVisitor& p_visitor) override;

 private:
  ExpressionNode* expr;
};

#endif
