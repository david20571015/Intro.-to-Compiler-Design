#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include <string>
#include <vector>

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class VariableReferenceNode : public ExpressionNode {
 public:
  // normal reference
  VariableReferenceNode(const uint32_t line, const uint32_t col,
                        const char *const name);
  // array reference
  VariableReferenceNode(const uint32_t line, const uint32_t col,
                        const char *const name,
                        std::vector<ExpressionNode *> *dims);
  ~VariableReferenceNode();

  const char *getNameCString() const;
  void accept(AstNodeVisitor &p_visitor) override;
  void visitChildNodes(AstNodeVisitor &p_visitor) override;

 private:
  std::string name;
  std::vector<ExpressionNode *> *dims;
};

#endif
