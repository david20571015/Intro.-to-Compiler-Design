#include "AST/for.hpp"

ForNode::ForNode(const uint32_t line, const uint32_t col, DeclNode *decl,
                 AssignmentNode *init, ConstantValueNode *condition,
                 CompoundStatementNode *body)
    : AstNode{line, col},
      decl(decl),
      init(init),
      condition(condition),
      body(body) {}

ForNode::~ForNode() {
  delete decl;
  delete init;
  delete condition;
  delete body;
}

void ForNode::accept(AstNodeVisitor &p_visitor) { p_visitor.visit(*this); }

void ForNode::visitChildNodes(AstNodeVisitor &p_visitor) {
  decl->accept(p_visitor);
  init->accept(p_visitor);
  condition->accept(p_visitor);
  body->accept(p_visitor);
}
