#ifndef AST_PROGRAM_NODE_H
#define AST_PROGRAM_NODE_H

#include <string>
#include <vector>

#include "AST/function.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "visitor/AstNodeVisitor.hpp"

class ProgramNode final : public AstNode {
 public:
  ProgramNode(const uint32_t line, const uint32_t col, const char *const p_name,
              std::vector<DeclNode *> *declarations,
              std::vector<FunctionNode *> *functions,
              CompoundStatementNode *compound_statement);
  ~ProgramNode() = default;

  // visitor pattern version:
  const char *getNameCString() const;
  void accept(AstNodeVisitor &p_visitor) override;
  void visitChildNodes(AstNodeVisitor &p_visitor) override;

 private:
  std::string name;
  std::vector<DeclNode *> *declarations;
  std::vector<FunctionNode *> *functions;
  CompoundStatementNode *compound_statement;
};

#endif
