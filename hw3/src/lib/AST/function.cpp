#include "AST/function.hpp"

// TODO
FunctionNode::FunctionNode(const uint32_t line, const uint32_t col,
                           const char* const name,
                           std::vector<DeclNode*>* declarations,
                           DataType* return_type,
                           CompoundStatementNode* compound)
    : AstNode{line, col},
      name(name),
      declarations(declarations),
      return_type(return_type),
      compound(compound) {}

FunctionNode::~FunctionNode() {
  for (auto& declaration : *declarations) {
    delete declaration;
  }
  delete declarations;
  delete return_type;
  if (compound != nullptr) {
    delete compound;
  }
}

const char* FunctionNode::getNameCString() const { return name.c_str(); }

const char* FunctionNode::getPrototypeCString() {
  if (proto_str.empty()) {
    setProtoStr();
  }
  return proto_str.c_str();
}

void FunctionNode::accept(AstNodeVisitor& p_visitor) { p_visitor.visit(*this); }

void FunctionNode::visitChildNodes(AstNodeVisitor& p_visitor) {
  for (auto& declaration : *declarations) {
    declaration->accept(p_visitor);
  }
  if (compound != nullptr) {
    compound->accept(p_visitor);
  }
}

void FunctionNode::setProtoStr() {
  proto_str = return_type->getTypeCString();
  proto_str += " (";
  for (auto& declaration : *declarations) {
    proto_str += declaration->getTypeCString();
    proto_str += ", ";
  }
  if (!declarations->empty()) {
    proto_str.pop_back();
    proto_str.pop_back();
  }
  proto_str += ")";
}