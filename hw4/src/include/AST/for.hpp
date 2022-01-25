#ifndef AST_FOR_NODE_H
#define AST_FOR_NODE_H

#include "AST/CompoundStatement.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/assignment.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/expression.hpp"

class ForNode final : public AstNode {
 private:
  std::unique_ptr<DeclNode> m_loop_var_decl;
  std::unique_ptr<AssignmentNode> m_init_stmt;
  std::unique_ptr<ExpressionNode> m_end_condition;
  std::unique_ptr<CompoundStatementNode> m_body;

 public:
  ~ForNode() = default;
  ForNode(const uint32_t line, const uint32_t col, DeclNode *p_loop_var_decl,
          AssignmentNode *p_init_stmt, ExpressionNode *p_end_condition,
          CompoundStatementNode *p_body)
      : AstNode{line, col},
        m_loop_var_decl(p_loop_var_decl),
        m_init_stmt(p_init_stmt),
        m_end_condition(p_end_condition),
        m_body(p_body) {}

  const ConstantValueNode *getInitialValue() const {
    return dynamic_cast<const ConstantValueNode *>(m_init_stmt->getExpr());
  }
  const ConstantValueNode *getFinalValue() const {
    return dynamic_cast<const ConstantValueNode *>(m_end_condition.get());
  }

  void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
  void visitChildNodes(AstNodeVisitor &p_visitor) override;
  void visitLoopVar(AstNodeVisitor &p_visitor);
  void visitBody(AstNodeVisitor &p_visitor);
};

#endif
