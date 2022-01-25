#ifndef __AST_ASSIGNMENT_NODE_H
#define __AST_ASSIGNMENT_NODE_H

#include "AST/VariableReference.hpp"
#include "AST/ast.hpp"
#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class AssignmentNode : public AstNode {
 public:
  AssignmentNode(const uint32_t line, const uint32_t col,
                 VariableReferenceNode* lvalue, ExpressionNode* expr);
  ~AssignmentNode();

  void accept(AstNodeVisitor& p_visitor) override;
  void visitChildNodes(AstNodeVisitor& p_visitor) override;

 private:
  VariableReferenceNode* lvalue;
  ExpressionNode* expr;
};

#endif
