#ifndef __AST_READ_NODE_H
#define __AST_READ_NODE_H

#include "AST/VariableReference.hpp"
#include "AST/ast.hpp"
#include "visitor/AstNodeVisitor.hpp"

class ReadNode : public AstNode {
 public:
  ReadNode(const uint32_t line, const uint32_t col,
           VariableReferenceNode* target);
  ~ReadNode();

  void accept(AstNodeVisitor& p_visitor) override;
  void visitChildNodes(AstNodeVisitor& p_visitor) override;

 private:
  VariableReferenceNode* target;
};

#endif
