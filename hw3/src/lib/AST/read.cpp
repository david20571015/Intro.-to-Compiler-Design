#include "AST/read.hpp"

ReadNode::ReadNode(const uint32_t line, const uint32_t col,
                   VariableReferenceNode *target)
    : AstNode{line, col}, target(target) {}

ReadNode::~ReadNode() { delete target; }

void ReadNode::accept(AstNodeVisitor &p_visitor) { p_visitor.visit(*this); }

void ReadNode::visitChildNodes(AstNodeVisitor &p_visitor) {
  target->accept(p_visitor);
}
