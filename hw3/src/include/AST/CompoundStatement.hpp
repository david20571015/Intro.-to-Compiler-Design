#ifndef __AST_COMPOUND_STATEMENT_NODE_H
#define __AST_COMPOUND_STATEMENT_NODE_H

#include <vector>

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "visitor/AstNodeVisitor.hpp"

class CompoundStatementNode : public AstNode {
 public:
  CompoundStatementNode(const uint32_t line, const uint32_t col,
                        std::vector<DeclNode *> *declarations,
                        std::vector<AstNode *> *statements);
  ~CompoundStatementNode();

  void accept(AstNodeVisitor &p_visitor) override;
  void visitChildNodes(AstNodeVisitor &p_visitor) override;

 private:
  std::vector<DeclNode *> *declarations;
  std::vector<AstNode *> *statements;
};

#endif
