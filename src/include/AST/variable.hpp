#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include <string>

#include "AST/AstType.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/ast.hpp"

class VariableNode : public AstNode {
 public:
  VariableNode(const uint32_t line, const uint32_t col, const char* const name);
  ~VariableNode();

  void setType(DataType* type);
  void setConstant(ConstantValueNode* constant);

  const char* getNameCString() const;
  const char* getTypeCString() const;
  void accept(AstNodeVisitor& p_visitor) override;
  void visitChildNodes(AstNodeVisitor& p_visitor) override;

 private:
  std::string name;
  DataType* type;
  ConstantValueNode* constant;
};

#endif
