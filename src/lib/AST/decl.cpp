#include "AST/decl.hpp"

DeclNode::DeclNode(const uint32_t line, const uint32_t col,
                   std::vector<VariableNode*>* variables)
    : AstNode{line, col}, variables(variables) {}

DeclNode::~DeclNode() {
  for (auto& variable : *variables) {
    delete variable;
  }
  delete variables;
}

const char* DeclNode::getTypeCString() {
  if (type_str.empty()) setTypeStr();
  return type_str.c_str();
};

void DeclNode::accept(AstNodeVisitor& p_visitor) { p_visitor.visit(*this); }

void DeclNode::visitChildNodes(AstNodeVisitor& p_visitor) {
  for (auto& variable : *variables) {
    variable->accept(p_visitor);
  }
}

void DeclNode::setTypeStr() {
  for (auto& variable : *variables) {
    type_str += variable->getTypeCString();
    type_str += ", ";
  }
  if (!type_str.empty()) {
    type_str.pop_back();
    type_str.pop_back();
  }
};