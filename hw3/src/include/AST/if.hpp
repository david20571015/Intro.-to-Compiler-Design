#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include "AST/CompoundStatement.hpp"
#include "AST/ast.hpp"
#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class IfNode : public AstNode {
 public:
  IfNode(const uint32_t line, const uint32_t col, ExpressionNode* condition,
         CompoundStatementNode* body_of_true,
         CompoundStatementNode* body_of_false);
  ~IfNode();

  void accept(AstNodeVisitor& p_visitor) override;
  void visitChildNodes(AstNodeVisitor& p_visitor) override;

 private:
  ExpressionNode* condition;
  CompoundStatementNode *body_of_true, *body_of_false;
};

#endif
