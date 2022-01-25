#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include <string>
#include <vector>

#include "AST/AstType.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"

class FunctionNode : public AstNode {
 public:
  FunctionNode(const uint32_t line, const uint32_t col, const char* const name,
               std::vector<DeclNode*>* declarations, DataType* return_type,
               CompoundStatementNode* compound = nullptr);
  ~FunctionNode();

  const char* getNameCString() const;
  const char* getPrototypeCString();
  void accept(AstNodeVisitor& p_visitor) override;
  void visitChildNodes(AstNodeVisitor& p_visitor) override;

 private:
  std::string name;
  std::vector<DeclNode*>* declarations;
  DataType* return_type;
  CompoundStatementNode* compound;
  std::string proto_str = "";

  void setProtoStr();
};

#endif
