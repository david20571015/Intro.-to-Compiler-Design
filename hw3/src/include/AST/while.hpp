#ifndef __AST_WHILE_NODE_H
#define __AST_WHILE_NODE_H

#include "AST/CompoundStatement.hpp"
#include "AST/ast.hpp"
#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class WhileNode : public AstNode {
 public:
  WhileNode(const uint32_t line, const uint32_t col, ExpressionNode* condition,
            CompoundStatementNode* body);
  ~WhileNode();

  void accept(AstNodeVisitor& p_visitor) override;
  void visitChildNodes(AstNodeVisitor& p_visitor) override;

 private:
  ExpressionNode* condition;
  CompoundStatementNode* body;
};

#endif
