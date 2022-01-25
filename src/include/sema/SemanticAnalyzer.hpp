#ifndef SEMA_SEMANTIC_ANALYZER_H
#define SEMA_SEMANTIC_ANALYZER_H

#include <stack>
#include <string>
#include <vector>

#include "AST/ast.hpp"
#include "SymbolTable.hpp"
#include "visitor/AstNodeVisitor.hpp"

struct ErrorMessage {
  Location location;
  std::string msg;

  ErrorMessage(const Location &location, const std::string &msg)
      : location(location), msg(msg) {}
  ~ErrorMessage() = default;
};

class SemanticAnalyzer final : public AstNodeVisitor {
 private:
  bool dump_table;
  SymbolManager symbol_manager;

  std::vector<ErrorMessage> errors;

  SymbolEntry::SymbolKind current_symbol_kind;
  PTypeSharedPtr current_type;
  SymbolEntry::Attribute current_attr;
  bool is_in_loop_body = false;
  std::stack<SymbolEntry *> context_manager;

 public:
  ~SemanticAnalyzer() = default;
  SemanticAnalyzer(bool dump_table) : dump_table(dump_table){};

  bool hasErrors() const { return !errors.empty(); }
  void dumpErrors(const char *srcPath);

  void visit(ProgramNode &p_program) override;
  void visit(DeclNode &p_decl) override;
  void visit(VariableNode &p_variable) override;
  void visit(ConstantValueNode &p_constant_value) override;
  void visit(FunctionNode &p_function) override;
  void visit(CompoundStatementNode &p_compound_statement) override;
  void visit(PrintNode &p_print) override;
  void visit(BinaryOperatorNode &p_bin_op) override;
  void visit(UnaryOperatorNode &p_un_op) override;
  void visit(FunctionInvocationNode &p_func_invocation) override;
  void visit(VariableReferenceNode &p_variable_ref) override;
  void visit(AssignmentNode &p_assignment) override;
  void visit(ReadNode &p_read) override;
  void visit(IfNode &p_if) override;
  void visit(WhileNode &p_while) override;
  void visit(ForNode &p_for) override;
  void visit(ReturnNode &p_return) override;
};

#endif
