#ifndef AST_OPERATOR_H
#define AST_OPERATOR_H

enum class Operator : uint8_t {
  kNegOp,
  kMultiplyOp,
  kDivideOp,
  kModOp,
  kPlusOp,
  kMinusOp,
  kLessOp,
  kLessOrEqualOp,
  kGreaterOp,
  kGreaterOrEqualOp,
  kEqualOp,
  kNotEqualOp,
  kNotOp,
  kAndOp,
  kOrOp
};

extern const char *kOpString[];

extern const char *kOpAssembly[];

#endif
