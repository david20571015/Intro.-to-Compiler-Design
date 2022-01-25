#ifndef AST_IF_NODE_H
#define AST_IF_NODE_H

#include <memory>

#include "AST/CompoundStatement.hpp"
#include "AST/ast.hpp"
#include "AST/expression.hpp"

class IfNode final : public AstNode {
 private:
  std::unique_ptr<ExpressionNode> m_condition;
  std::unique_ptr<CompoundStatementNode> m_body;
  std::unique_ptr<CompoundStatementNode> m_else_body;

 public:
  ~IfNode() = default;
  IfNode(const uint32_t line, const uint32_t col, ExpressionNode *p_condition,
         CompoundStatementNode *p_body, CompoundStatementNode *p_else_body)
      : AstNode{line, col},
        m_condition(p_condition),
        m_body(p_body),
        m_else_body(p_else_body) {}

  const ExpressionNode &getCondition() const { return *m_condition.get(); }
  const CompoundStatementNode &getBody() const { return *m_body.get(); }
  const CompoundStatementNode &getElseBody() const {
    return *m_else_body.get();
  }

  void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
  void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif
