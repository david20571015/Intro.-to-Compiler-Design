#include "AST/VariableReference.hpp"

VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col,
                                             const char *const name)
    : ExpressionNode{line, col},
      name(name),
      dims(new std::vector<ExpressionNode *>()) {}

VariableReferenceNode::VariableReferenceNode(
    const uint32_t line, const uint32_t col, const char *const name,
    std::vector<ExpressionNode *> *dims)
    : ExpressionNode{line, col}, name(name), dims(dims) {}

VariableReferenceNode::~VariableReferenceNode() {
  for (auto &dim : *dims) {
    delete dim;
  }
  delete dims;
}

const char *VariableReferenceNode::getNameCString() const {
  return name.c_str();
}

void VariableReferenceNode::accept(AstNodeVisitor &p_visitor) {
  p_visitor.visit(*this);
}

void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
  for (auto &dim : *dims) {
    dim->accept(p_visitor);
  }
}
