#include "AST/AstType.hpp"

DataType::DataType(ScalarDataType type) : type(type), dims(nullptr) {}

DataType::DataType(ScalarDataType type, std::vector<int> *dims)
    : type(type), dims(dims) {}

DataType::~DataType() {
  if (dims != nullptr) {
    delete dims;
  }
}

ScalarDataType DataType::getType() const { return type; }

std::vector<int> *DataType::getDims() const { return dims; }

const char *DataType::getTypeCString() {
  if (type_str.empty()) {
    setTypeStr();
  }
  return type_str.c_str();
}

void DataType::setTypeStr() {
  switch (type) {
    case ScalarDataType::INT_TYPE:
      type_str = "integer";
      break;
    case ScalarDataType::REAL_TYPE:
      type_str = "real";
      break;
    case ScalarDataType::BOOL_TYPE:
      type_str = "boolean";
      break;
    case ScalarDataType::STRING_TYPE:
      type_str = "string";
      break;
    case ScalarDataType::VOID:
      type_str = "void";
      break;
  }

  if (dims != nullptr) {
    type_str += " ";
    for (int dim : *dims) {
      type_str += "[";
      type_str += std::to_string(dim);
      type_str += "]";
    }
  }
}