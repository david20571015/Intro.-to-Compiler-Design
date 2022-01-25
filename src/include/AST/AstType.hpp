#ifndef AST_AST_TYPE_H
#define AST_AST_TYPE_H

#include <string>
#include <vector>

enum ScalarDataType { INT_TYPE, REAL_TYPE, BOOL_TYPE, STRING_TYPE, VOID };

union ScalarValue {
  int int_value;
  float real_value;
  bool bool_value;
  const char *string_value;
};

class DataType {
 public:
  DataType(ScalarDataType type);
  DataType(ScalarDataType type, std::vector<int> *dims);
  ~DataType();

  ScalarDataType getType() const;
  std::vector<int> *getDims() const;
  const char *getTypeCString();

 private:
  ScalarDataType type;
  std::vector<int> *dims;
  std::string type_str = "";

  void setTypeStr();
};

#endif