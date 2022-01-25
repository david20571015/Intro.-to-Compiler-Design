#include "AST/while.hpp"

WhileNode::WhileNode(const uint32_t line, const uint32_t col,
                     ExpressionNode* condition, CompoundStatementNode* body)
    : AstNode{line, col}, condition(condition), body(body) {}

WhileNode::~WhileNode() {
  delete condition;
  delete body;
}

void WhileNode::accept(AstNodeVisitor& p_visitor) { p_visitor.visit(*this); }

void WhileNode::visitChildNodes(AstNodeVisitor& p_visitor) {
  condition->accept(p_visitor);
  body->accept(p_visitor);
}
