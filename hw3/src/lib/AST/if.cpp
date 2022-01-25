#include "AST/if.hpp"

// TODO
IfNode::IfNode(const uint32_t line, const uint32_t col,
               ExpressionNode* condition, CompoundStatementNode* body_of_true,
               CompoundStatementNode* body_of_false)
    : AstNode{line, col},
      condition(condition),
      body_of_true(body_of_true),
      body_of_false(body_of_false) {}

IfNode::~IfNode() {
  delete condition;
  delete body_of_true;
  if (body_of_true != nullptr) {
    delete body_of_false;
  }
}

void IfNode::accept(AstNodeVisitor& p_visitor) { p_visitor.visit(*this); }

void IfNode::visitChildNodes(AstNodeVisitor& p_visitor) {
  condition->accept(p_visitor);
  body_of_true->accept(p_visitor);
  if (body_of_false != nullptr) {
    body_of_false->accept(p_visitor);
  }
}
