#include "AST/variable.hpp"

// TODO
VariableNode::VariableNode(const uint32_t line, const uint32_t col,
                           const char* const name)
    : AstNode{line, col}, name(name), type(nullptr), constant(nullptr) {}

VariableNode::~VariableNode() {
  delete type;
  if (constant != nullptr) {
    delete constant;
  }
}

void VariableNode::setType(DataType* type) { this->type = type; }

void VariableNode::setConstant(ConstantValueNode* constant) {
  this->type = constant->getDataType();
  this->constant = constant;
}

const char* VariableNode::getNameCString() const { return name.c_str(); }

const char* VariableNode::getTypeCString() const {
  return type->getTypeCString();
}

void VariableNode::accept(AstNodeVisitor& p_visitor) { p_visitor.visit(*this); }

void VariableNode::visitChildNodes(AstNodeVisitor& p_visitor) {
  if (constant != nullptr) {
    constant->accept(p_visitor);
  }
}
