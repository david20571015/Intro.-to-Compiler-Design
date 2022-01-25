#include "AST/BinaryOperator.hpp"

BinaryOperatorNode::BinaryOperatorNode(const uint32_t line, const uint32_t col,
                                       ExpressionNode *left, Operator *op,
                                       ExpressionNode *right)
    : ExpressionNode{line, col}, left(left), right(right), op(op) {}

BinaryOperatorNode::~BinaryOperatorNode() {
  delete left;
  delete op;
  delete right;
}

const char *BinaryOperatorNode::getOperatorCString() const {
  return op->getTypeCString();
}

void BinaryOperatorNode::accept(AstNodeVisitor &p_visitor) {
  p_visitor.visit(*this);
}

void BinaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
  left->accept(p_visitor);
  right->accept(p_visitor);
}
