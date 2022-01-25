#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include <vector>

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class FunctionInvocationNode : public ExpressionNode {
 public:
  FunctionInvocationNode(const uint32_t line, const uint32_t col,
                         const char* const name,
                         std::vector<ExpressionNode*>* exprs);
  ~FunctionInvocationNode();

  const char* getNameCString() const;
  void accept(AstNodeVisitor& p_visitor) override;
  void visitChildNodes(AstNodeVisitor& p_visitor) override;

 private:
  std::string name;
  std::vector<ExpressionNode*>* exprs;
};

#endif
