#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include <string>

#include "AST/AstType.hpp"
#include "AST/expression.hpp"

class ConstantValueNode : public ExpressionNode {
 public:
  ConstantValueNode(const uint32_t line, const uint32_t col, int int_value);
  ConstantValueNode(const uint32_t line, const uint32_t col, float real_value);
  ConstantValueNode(const uint32_t line, const uint32_t col, bool bool_value);
  ConstantValueNode(const uint32_t line, const uint32_t col,
                    const char *const string_value);
  ~ConstantValueNode() = default;

  DataType *getDataType();
  const char *getValueCString() const;
  void accept(AstNodeVisitor &p_visitor) override;
  void visitChildNodes(AstNodeVisitor &p_visitor) override;

 private:
  int int_value = 0;
  float real_value = 0.0f;
  bool bool_value = false;
  std::string string_value;

  DataType type;
};

#endif
