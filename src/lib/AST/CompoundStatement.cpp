#include "AST/CompoundStatement.hpp"

CompoundStatementNode::CompoundStatementNode(
    const uint32_t line, const uint32_t col,
    std::vector<DeclNode *> *declarations, std::vector<AstNode *> *statements)
    : AstNode{line, col}, declarations(declarations), statements(statements) {}

CompoundStatementNode::~CompoundStatementNode() {
  for (auto &decl : *declarations) {
    delete decl;
  }
  delete declarations;
  for (auto &stmt : *statements) {
    delete stmt;
  }
  delete statements;
}

void CompoundStatementNode::accept(AstNodeVisitor &p_visitor) {
  p_visitor.visit(*this);
}

void CompoundStatementNode::visitChildNodes(AstNodeVisitor &p_visitor) {
  for (auto &decl : *declarations) {
    decl->accept(p_visitor);
  }
  for (auto &stmt : *statements) {
    stmt->accept(p_visitor);
  }
}
