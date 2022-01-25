#ifndef __AST_PRINT_NODE_H
#define __AST_PRINT_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class PrintNode : public AstNode {
 public:
  PrintNode(const uint32_t line, const uint32_t col, ExpressionNode* expr);
  ~PrintNode();

  void accept(AstNodeVisitor& p_visitor) override;
  void visitChildNodes(AstNodeVisitor& p_visitor) override;

 private:
  ExpressionNode* expr;
};

#endif
