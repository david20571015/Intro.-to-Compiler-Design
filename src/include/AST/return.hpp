#ifndef AST_RETURN_NODE_H
#define AST_RETURN_NODE_H

#include <memory>

#include "AST/ast.hpp"
#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class ReturnNode final : public AstNode {
 private:
  std::unique_ptr<ExpressionNode> m_ret_val;

 public:
  ~ReturnNode() = default;
  ReturnNode(const uint32_t line, const uint32_t col, ExpressionNode *p_ret_val)
      : AstNode{line, col}, m_ret_val(p_ret_val) {}

  const ExpressionNode *getRetVal() const { return m_ret_val.get(); }

  void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
  void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif
