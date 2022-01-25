#include "AST/return.hpp"

ReturnNode::ReturnNode(const uint32_t line, const uint32_t col,
                       ExpressionNode* expr)
    : AstNode{line, col}, expr(expr) {}

ReturnNode::~ReturnNode() { delete expr; }

void ReturnNode::accept(AstNodeVisitor& p_visitor) { p_visitor.visit(*this); }

void ReturnNode::visitChildNodes(AstNodeVisitor& p_visitor) {
  expr->accept(p_visitor);
}
