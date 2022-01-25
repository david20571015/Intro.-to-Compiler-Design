#include "AST/print.hpp"

PrintNode::PrintNode(const uint32_t line, const uint32_t col,
                     ExpressionNode* expr)
    : AstNode{line, col}, expr(expr) {}

PrintNode::~PrintNode() { delete expr; }

void PrintNode::accept(AstNodeVisitor& p_visitor) { p_visitor.visit(*this); }

void PrintNode::visitChildNodes(AstNodeVisitor& p_visitor) {
  expr->accept(p_visitor);
}
