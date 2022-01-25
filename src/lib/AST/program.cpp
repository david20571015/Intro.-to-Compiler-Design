#include "AST/program.hpp"

ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
                         const char *const p_name,
                         std::vector<DeclNode *> *declarations,
                         std::vector<FunctionNode *> *functions,
                         CompoundStatementNode *compound_statement)
    : AstNode{line, col},
      name(p_name),
      declarations(declarations),
      functions(functions),
      compound_statement(compound_statement) {}

// visitor pattern version:
const char *ProgramNode::getNameCString() const { return name.c_str(); }

void ProgramNode::accept(AstNodeVisitor &p_visitor) { p_visitor.visit(*this); }

void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) {
  for (auto &decl : *declarations) {
    decl->accept(p_visitor);
  }

  for (auto &func : *functions) {
    func->accept(p_visitor);
  }

  compound_statement->accept(p_visitor);
}
