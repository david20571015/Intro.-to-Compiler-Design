#include "AST/assignment.hpp"

AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col,
                               VariableReferenceNode* lvalue,
                               ExpressionNode* expr)
    : AstNode{line, col}, lvalue(lvalue), expr(expr) {}

AssignmentNode::~AssignmentNode() {
  delete lvalue;
  delete expr;
}

void AssignmentNode::accept(AstNodeVisitor& p_visitor) {
  p_visitor.visit(*this);
}

void AssignmentNode::visitChildNodes(AstNodeVisitor& p_visitor) {
  lvalue->accept(p_visitor);
  expr->accept(p_visitor);
}
