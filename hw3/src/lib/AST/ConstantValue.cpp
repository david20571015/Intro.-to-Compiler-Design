#include "AST/ConstantValue.hpp"

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     int int_value)
    : ExpressionNode{line, col},
      int_value(int_value),
      type(DataType(ScalarDataType::INT_TYPE)) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     float real_value)
    : ExpressionNode{line, col},
      real_value(real_value),
      type(DataType(ScalarDataType::REAL_TYPE)) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     bool bool_value)
    : ExpressionNode{line, col},
      bool_value(bool_value),
      type(DataType(ScalarDataType::BOOL_TYPE)) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     const char* const string_value)
    : ExpressionNode{line, col},
      string_value(string_value),
      type(DataType(ScalarDataType::STRING_TYPE)) {}

DataType* ConstantValueNode::getDataType() { return &type; }

const char* ConstantValueNode::getValueCString() const {
  switch (type.getType()) {
    case ScalarDataType::INT_TYPE:
      return std::to_string(int_value).c_str();
    case ScalarDataType::REAL_TYPE:
      return std::to_string(real_value).c_str();
    case ScalarDataType::BOOL_TYPE:
      return bool_value ? "true" : "false";
    case ScalarDataType::STRING_TYPE:
      return string_value.c_str();
    default:
      return "";
  }
}

void ConstantValueNode::accept(AstNodeVisitor& p_visitor) {
  p_visitor.visit(*this);
}

void ConstantValueNode::visitChildNodes(AstNodeVisitor& p_visitor) {}
