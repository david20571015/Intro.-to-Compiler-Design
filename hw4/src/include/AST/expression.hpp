#ifndef AST_EXPRESSION_NODE_H
#define AST_EXPRESSION_NODE_H

#include "AST/ast.hpp"
#include "AST/PType.hpp"

class ExpressionNode : public AstNode {
 public:
  ~ExpressionNode() = default;
  ExpressionNode(const uint32_t line, const uint32_t col)
      : AstNode{line, col} {}

  const PType *getReturnType() const { return type.get(); }
  void setReturnType(PType *p_type) { type.reset(p_type); }
  void setReturnType(const PType::PrimitiveTypeEnum p_type) {
    type.reset(new PType(p_type));
  }

 protected:
  // for carrying type of result of an expression
  PTypeSharedPtr type = nullptr;
};

#endif
