#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/CompoundStatement.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/assignment.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "visitor/AstNodeVisitor.hpp"

class ForNode : public AstNode {
 public:
  ForNode(const uint32_t line, const uint32_t col, DeclNode *decl,
          AssignmentNode *init, ConstantValueNode *condition,
          CompoundStatementNode *body);
  ~ForNode();

  void accept(AstNodeVisitor &p_visitor) override;
  void visitChildNodes(AstNodeVisitor &p_visitor) override;

 private:
  DeclNode *decl;
  AssignmentNode *init;
  ConstantValueNode *condition;
  CompoundStatementNode *body;
};

#endif
