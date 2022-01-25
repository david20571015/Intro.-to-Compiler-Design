#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include <vector>

#include "AST/ast.hpp"
#include "AST/variable.hpp"

class DeclNode : public AstNode {
 public:
  DeclNode(const uint32_t line, const uint32_t col,
           std::vector<VariableNode *> *variables);
  ~DeclNode();

  const char *getTypeCString();
  void accept(AstNodeVisitor &p_visitor) override;
  void visitChildNodes(AstNodeVisitor &p_visitor) override;

 private:
  std::vector<VariableNode *> *variables;
  std::string type_str = "";

  void setTypeStr();
};

#endif
