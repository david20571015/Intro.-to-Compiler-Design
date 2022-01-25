#include "AST/UnaryOperator.hpp"

UnaryOperatorNode::UnaryOperatorNode(const uint32_t line, const uint32_t col,
                                     Operator *op, ExpressionNode *expr)
    : ExpressionNode{line, col}, op(op), expr(expr) {}

UnaryOperatorNode::~UnaryOperatorNode() {
  delete op;
  delete expr;
}

const char *UnaryOperatorNode::getOperatorCString() const {
  return op->getTypeCString();
}

void UnaryOperatorNode::accept(AstNodeVisitor &p_visitor) {
  p_visitor.visit(*this);
}

void UnaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
  expr->accept(p_visitor);
}
