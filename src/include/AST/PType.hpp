#ifndef AST_P_TYPE_H
#define AST_P_TYPE_H

#include <memory>
#include <string>
#include <vector>

class PType;

using PTypeSharedPtr = std::shared_ptr<PType>;

class PType {
 public:
  enum class PrimitiveTypeEnum : uint8_t {
    kVoidType,
    kIntegerType,
    kRealType,
    kBoolType,
    kStringType
  };

 private:
  PrimitiveTypeEnum m_type;
  std::vector<uint64_t> m_dimensions;
  mutable std::string m_type_string;
  mutable bool m_type_string_is_valid = false;

 public:
  ~PType() = default;
  PType(const PrimitiveTypeEnum type) : m_type(type) {}

  void setDimensions(std::vector<uint64_t> p_dims) { m_dimensions = p_dims; }

  const std::vector<uint64_t> &getDimensions() const { return m_dimensions; }

  PrimitiveTypeEnum getPrimitiveType() const { return m_type; }
  PTypeSharedPtr getStructuredType(size_t dims) const;
  const char *getPTypeCString() const;

  bool operator==(const PType &p_type) const;
  bool operator!=(const PType &p_type) const { return !(*this == p_type); };

  bool isVoid() const { return m_type == PrimitiveTypeEnum::kVoidType; }
  bool isPrimitiveInteger() const {
    return m_type == PrimitiveTypeEnum::kIntegerType;
  }
  bool isPrimitiveReal() const {
    return m_type == PrimitiveTypeEnum::kRealType;
  }
  bool isPrimitiveBool() const {
    return m_type == PrimitiveTypeEnum::kBoolType;
  }
  bool isPrimitiveString() const {
    return m_type == PrimitiveTypeEnum::kStringType;
  }

  bool isScalar() const {
    return m_dimensions.empty() && m_type != PrimitiveTypeEnum::kVoidType;
  }
  bool isArray() const { return !m_dimensions.empty(); }

  bool isInteger() const { return isScalar() && isPrimitiveInteger(); }
  bool isReal() const { return isScalar() && isPrimitiveReal(); }
  bool isBool() const { return isScalar() && isPrimitiveBool(); }
  bool isString() const { return isScalar() && isPrimitiveString(); }
};

#endif
