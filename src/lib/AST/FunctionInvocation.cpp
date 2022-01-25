#include "AST/FunctionInvocation.hpp"

FunctionInvocationNode::FunctionInvocationNode(
    const uint32_t line, const uint32_t col, const char* const name,
    std::vector<ExpressionNode*>* exprs)
    : ExpressionNode{line, col}, name(name), exprs(exprs) {}

FunctionInvocationNode::~FunctionInvocationNode() {
  for (auto expr : *exprs) {
    delete expr;
  }
  delete exprs;
}

const char* FunctionInvocationNode::getNameCString() const {
  return name.c_str();
}

void FunctionInvocationNode::accept(AstNodeVisitor& p_visitor) {
  p_visitor.visit(*this);
}

void FunctionInvocationNode::visitChildNodes(AstNodeVisitor& p_visitor) {
  for (auto expr : *exprs) {
    expr->accept(p_visitor);
  }
}
