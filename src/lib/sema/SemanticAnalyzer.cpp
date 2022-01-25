#include "sema/SemanticAnalyzer.hpp"

#include <algorithm>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include "AST/PType.hpp"
#include "AST/operator.hpp"
#include "visitor/AstNodeInclude.hpp"

static inline void dumpError(const ErrorMessage &error,
                             const std::string &src) {
  fprintf(stderr, "<Error> Found in line %u, column %u: %s\n",
          error.location.line, error.location.col, error.msg.c_str());
  fprintf(stderr, "    %s\n", src.c_str());
  fprintf(stderr, "    %*c\n", error.location.col, '^');
}

void SemanticAnalyzer::dumpErrors(const char *srcPath) {
  std::ifstream srcFile(srcPath, std::ios::in);

  std::vector<std::string> src;
  std::string line;
  while (std::getline(srcFile, line)) {
    src.push_back(line);
  }

  srcFile.close();

  for (auto &error : errors) {
    dumpError(error, src[error.location.line - 1]);
  }
}

void SemanticAnalyzer::visit(ProgramNode &p_program) {
  symbol_manager.enterScope();
  current_symbol_kind = SymbolEntry::SymbolKind::kProgram;

  PTypeSharedPtr type_ptr =
      std::make_shared<PType>(PType::PrimitiveTypeEnum::kVoidType);
  auto symbol = symbol_manager.addSymbol(
      p_program.getLocation(), p_program.getNameCString(),
      SymbolEntry::SymbolKind::kProgram, type_ptr);

  context_manager.push(&symbol);
  p_program.visitChildNodes(*this);
  context_manager.pop();

  if (dump_table) symbol_manager.dumpTopTable();
  symbol_manager.exitScope();
}

void SemanticAnalyzer::visit(DeclNode &p_decl) {
  p_decl.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(VariableNode &p_variable) {
  p_variable.visitChildNodes(*this);

  auto target = symbol_manager.lookup(p_variable.getNameCString());
  if (target) {
    if (target->getLevel() == symbol_manager.getLevel() - 1 ||
        target->getKind() == SymbolEntry::SymbolKind::kLoopVar) {
      std::string msg = "symbol '" + std::string(p_variable.getNameCString()) +
                        "' is redeclared";
      errors.emplace_back(p_variable.getLocation(), msg);
      return;
    }
  }

  bool is_decl_error = false;
  const std::vector<uint64_t> &dims = p_variable.getType()->getDimensions();
  auto array_dim_illegal = [](uint64_t dim) { return dim <= 0; };
  if (std::any_of(dims.begin(), dims.end(), array_dim_illegal)) {
    std::string msg =
        "'" + std::string(p_variable.getNameCString()) +
        "' declared as an array with an index that is not greater than 0";
    errors.emplace_back(p_variable.getLocation(), msg);
    is_decl_error = true;
  }

  switch (current_symbol_kind) {
    case SymbolEntry::SymbolKind::kConstant:
      symbol_manager
          .addSymbol(p_variable.getLocation(), p_variable.getNameCString(),
                     SymbolEntry::SymbolKind::kConstant, current_type)
          .setConstantAttr(current_attr.constant);
      current_symbol_kind = SymbolEntry::SymbolKind::kVariable;
      break;
    case SymbolEntry::SymbolKind::kFunction:
      symbol_manager.addSymbol(
          p_variable.getLocation(), p_variable.getNameCString(),
          SymbolEntry::SymbolKind::kParameter, p_variable.getType());
      break;
    case SymbolEntry::SymbolKind::kLoopVar:
      symbol_manager.addSymbol(
          p_variable.getLocation(), p_variable.getNameCString(),
          SymbolEntry::SymbolKind::kLoopVar, p_variable.getType());
      break;
    default:
      symbol_manager
          .addSymbol(p_variable.getLocation(), p_variable.getNameCString(),
                     SymbolEntry::SymbolKind::kVariable, p_variable.getType())
          .setDeclError(is_decl_error);
      break;
  }
}

void SemanticAnalyzer::visit(ConstantValueNode &p_constant_value) {
  p_constant_value.setReturnType(
      p_constant_value.getTypeSharedPtr()->getPrimitiveType());

  current_symbol_kind = SymbolEntry::SymbolKind::kConstant;
  current_type = p_constant_value.getTypeSharedPtr();
  current_attr.constant = p_constant_value.getConstant();
}

void SemanticAnalyzer::visit(FunctionNode &p_function) {
  auto target = symbol_manager.lookup(p_function.getNameCString());

  SymbolEntry *symbol = nullptr;

  if (target && target->getLevel() == symbol_manager.getLevel() - 1) {
    std::string msg = "symbol '" + std::string(p_function.getNameCString()) +
                      "' is redeclared";
    errors.emplace_back(p_function.getLocation(), msg);
  } else {
    symbol = &(symbol_manager.addSymbol(
        p_function.getLocation(), p_function.getNameCString(),
        SymbolEntry::SymbolKind::kFunction, p_function.getReturnType()));
    symbol->setFunctionAttr(&p_function);
  }

  symbol_manager.enterScope();

  context_manager.push(symbol);
  current_symbol_kind = SymbolEntry::SymbolKind::kFunction;
  p_function.visitParameters(*this);

  current_symbol_kind = SymbolEntry::SymbolKind::kVariable;
  p_function.visitBody(*this);
  context_manager.pop();

  if (dump_table) symbol_manager.dumpTopTable();
  symbol_manager.exitScope();
}

void SemanticAnalyzer::visit(CompoundStatementNode &p_compound_statement) {
  symbol_manager.enterScope();

  p_compound_statement.visitChildNodes(*this);

  if (dump_table) symbol_manager.dumpTopTable();
  symbol_manager.exitScope();
}

void SemanticAnalyzer::visit(PrintNode &p_print) {
  p_print.visitChildNodes(*this);

  auto *return_type = p_print.getTarget()->getReturnType();

  if (return_type && !return_type->isScalar()) {
    std::string msg = "expression of print statement must be scalar type";
    errors.emplace_back(p_print.getTarget()->getLocation(), msg);
  }
}

void SemanticAnalyzer::visit(BinaryOperatorNode &p_bin_op) {
  p_bin_op.visitChildNodes(*this);

  auto left_type = p_bin_op.getLeftOperand()->getReturnType();
  auto right_type = p_bin_op.getRightOperand()->getReturnType();

  if (!left_type || !right_type) {
    return;
  }

  switch (p_bin_op.getOperator()) {
    // Arithmetic operator
    case Operator::kPlusOp: {
      if (left_type->isString() && right_type->isString()) {
        p_bin_op.setReturnType(PType::PrimitiveTypeEnum::kStringType);
        return;
      }
      [[fallthrough]];
    }
    case Operator::kMinusOp:
    case Operator::kMultiplyOp:
    case Operator::kDivideOp: {
      if (!(left_type->isInteger() || left_type->isReal()) ||
          !(right_type->isInteger() || right_type->isReal())) {
        break;
      }
      p_bin_op.setReturnType((left_type->isReal() || right_type->isReal())
                                 ? PType::PrimitiveTypeEnum::kRealType
                                 : PType::PrimitiveTypeEnum::kIntegerType);
      return;
    }
    // Mod operator
    case Operator::kModOp: {
      if (!left_type->isInteger() || !right_type->isInteger()) {
        break;
      }
      p_bin_op.setReturnType(PType::PrimitiveTypeEnum::kIntegerType);
      return;
    }
    // Boolean operator
    case Operator::kAndOp:
    case Operator::kOrOp: {
      if (!left_type->isBool() || !right_type->isBool()) {
        break;
      }
      p_bin_op.setReturnType(PType::PrimitiveTypeEnum::kBoolType);
      return;
    }
    // Relational operator
    case Operator::kLessOp:
    case Operator::kLessOrEqualOp:
    case Operator::kEqualOp:
    case Operator::kGreaterOp:
    case Operator::kGreaterOrEqualOp:
    case Operator::kNotEqualOp: {
      if (!(left_type->isInteger() || left_type->isReal()) ||
          !(right_type->isInteger() || right_type->isReal())) {
        break;
      }
      p_bin_op.setReturnType(PType::PrimitiveTypeEnum::kBoolType);
      return;
    }

    default:
      break;
  }

  std::string msg = "invalid operands to binary operator '" +
                    std::string(p_bin_op.getOpCString()) + "' ('" +
                    std::string(left_type->getPTypeCString()) + "' and '" +
                    std::string(right_type->getPTypeCString()) + "')";
  errors.emplace_back(p_bin_op.getLocation(), msg);
}

void SemanticAnalyzer::visit(UnaryOperatorNode &p_un_op) {
  p_un_op.visitChildNodes(*this);

  auto operand_type = p_un_op.getOperand()->getReturnType();

  if (!operand_type) {
    return;
  }

  switch (p_un_op.getOperator()) {
    // Neg operator
    case Operator::kNegOp: {
      if (!operand_type->isInteger() && !operand_type->isReal()) {
        break;
      }
      p_un_op.setReturnType(operand_type->isReal()
                                ? PType::PrimitiveTypeEnum::kRealType
                                : PType::PrimitiveTypeEnum::kIntegerType);
      return;
    }
    // Boolean operator
    case Operator::kNotOp: {
      if (!operand_type->isBool()) {
        break;
      }
      p_un_op.setReturnType(PType::PrimitiveTypeEnum::kBoolType);
      return;
    }

    default:
      break;
  }

  std::string msg = "invalid operand to unary operator '" +
                    std::string(p_un_op.getOpCString()) + "' ('" +
                    std::string(operand_type->getPTypeCString()) + "')";
  errors.emplace_back(p_un_op.getLocation(), msg);
}

void SemanticAnalyzer::visit(FunctionInvocationNode &p_func_invocation) {
  p_func_invocation.visitChildNodes(*this);

  auto target = symbol_manager.lookup(p_func_invocation.getNameCString());

  if (!target.has_value()) {
    std::string msg = "use of undeclared symbol '" +
                      std::string(p_func_invocation.getNameCString()) + "'";
    errors.emplace_back(p_func_invocation.getLocation(), msg);
    return;
  }

  if (target->getKind() != SymbolEntry::SymbolKind::kFunction) {
    std::string msg = "call of non-function symbol '" +
                      std::string(p_func_invocation.getNameCString()) + "'";
    errors.emplace_back(p_func_invocation.getLocation(), msg);
    return;
  }

  auto *func = target->getFunctionAttr();

  if (func->getParameters().size() != p_func_invocation.getArguments().size()) {
    std::string msg = "too few/much arguments provided for function '" +
                      std::string(p_func_invocation.getNameCString()) + "'";
    errors.emplace_back(p_func_invocation.getLocation(), msg);
    return;
  }

  for (size_t i = 0; i < func->getParameters().size(); ++i) {
    auto &param = func->getParameters()[i];
    auto &arg = p_func_invocation.getArguments()[i];

    auto *param_type = param->getVariables()[0]->getType().get();
    auto *arg_type = arg->getReturnType();

    // Type Coercion
    if (param_type->isReal() && arg_type->isInteger()) {
      continue;
    }

    if (*param_type != *arg_type) {
      std::string msg = "incompatible type passing '" +
                        std::string(arg_type->getPTypeCString()) +
                        "' to parameter of type '" +
                        std::string(param_type->getPTypeCString()) + "'";
      errors.emplace_back(arg->getLocation(), msg);
      return;
    }
  }

  p_func_invocation.setReturnType(target->getType().get());
}

void SemanticAnalyzer::visit(VariableReferenceNode &p_variable_ref) {
  p_variable_ref.visitChildNodes(*this);

  auto target = symbol_manager.lookup(p_variable_ref.getNameCString());

  if (!target.has_value()) {
    std::string msg = "use of undeclared symbol '" +
                      std::string(p_variable_ref.getNameCString()) + "'";
    errors.emplace_back(p_variable_ref.getLocation(), msg);
    return;
  }

  if (target->getKind() != SymbolEntry::SymbolKind::kParameter &&
      target->getKind() != SymbolEntry::SymbolKind::kVariable &&
      target->getKind() != SymbolEntry::SymbolKind::kLoopVar &&
      target->getKind() != SymbolEntry::SymbolKind::kConstant) {
    std::string msg = "use of non-variable symbol '" +
                      std::string(p_variable_ref.getNameCString()) + "'";
    errors.emplace_back(p_variable_ref.getLocation(), msg);
    return;
  }

  if (target->isDeclError()) {
    return;
  }

  for (auto &index : p_variable_ref.getIndices()) {
    if (index->getReturnType() == nullptr) {
      return;
    }
    if (!index->getReturnType()->isInteger()) {
      std::string msg = "index of array reference must be an integer";
      errors.emplace_back(index->getLocation(), msg);
      return;
    }
  }

  auto var_ref_type =
      target->getType()->getStructuredType(p_variable_ref.getIndices().size());
  if (!var_ref_type) {
    std::string msg = "there is an over array subscript on '" +
                      std::string(p_variable_ref.getNameCString()) + "'";
    errors.emplace_back(p_variable_ref.getLocation(), msg);
    return;
  }

  PType *p_type = new PType(target->getType()->getPrimitiveType());
  p_type->setDimensions(var_ref_type.get()->getDimensions());

  p_variable_ref.setReturnType(p_type);
}

void SemanticAnalyzer::visit(AssignmentNode &p_assignment) {
  p_assignment.visitChildNodes(*this);

  auto *l_value = p_assignment.getLValue();
  auto target = symbol_manager.lookup(l_value->getNameCString());

  if (!target.has_value()) {
    return;
  }

  if (l_value->getReturnType()->isArray()) {
    std::string msg = "array assignment is not allowed";
    errors.emplace_back(l_value->getLocation(), msg);
    return;
  }

  if (target->getKind() == SymbolEntry::SymbolKind::kConstant) {
    std::string msg = "cannot assign to variable '" +
                      std::string(l_value->getNameCString()) +
                      "' which is a constant";
    errors.emplace_back(l_value->getLocation(), msg);
    return;
  }

  if (is_in_loop_body &&
      target->getKind() == SymbolEntry::SymbolKind::kLoopVar) {
    std::string msg =
        "the value of loop variable cannot be modified inside the loop body";
    errors.emplace_back(l_value->getLocation(), msg);
    return;
  }

  auto *expr = p_assignment.getExpr();

  if (expr->getReturnType() == nullptr) {
    return;
  }

  if (expr->getReturnType()->isArray()) {
    std::string msg = "array assignment is not allowed";
    errors.emplace_back(expr->getLocation(), msg);
    return;
  }

  if (expr->getReturnType()->isInteger() && target->getType()->isReal()) {
    return;
  }

  if (*expr->getReturnType() != *target->getType()) {
    std::string msg = "assigning to '" +
                      std::string(l_value->getReturnType()->getPTypeCString()) +
                      "' from incompatible type '" +
                      std::string(expr->getReturnType()->getPTypeCString()) +
                      "'";
    errors.emplace_back(p_assignment.getLocation(), msg);
    return;
  }
}

void SemanticAnalyzer::visit(ReadNode &p_read) {
  p_read.visitChildNodes(*this);

  auto *return_type = p_read.getTarget()->getReturnType();

  if (return_type == nullptr) {
    return;
  }

  if (return_type->isArray()) {
    std::string msg =
        "variable reference of read statement must be scalar type";
    errors.emplace_back(p_read.getTarget()->getLocation(), msg);
  }

  auto target = symbol_manager.lookup(p_read.getTarget()->getNameCString());

  if (target->getKind() == SymbolEntry::SymbolKind::kConstant ||
      target->getKind() == SymbolEntry::SymbolKind::kLoopVar) {
    std::string msg =
        "variable reference of read statement cannot be a constant or loop "
        "variable";
    errors.emplace_back(p_read.getTarget()->getLocation(), msg);
  }
}

void SemanticAnalyzer::visit(IfNode &p_if) {
  p_if.visitChildNodes(*this);

  auto *condition = p_if.getCondition();

  if (condition->getReturnType() == nullptr) {
    return;
  }

  if (!condition->getReturnType()->isBool()) {
    std::string msg = "the expression of condition must be boolean type";
    errors.emplace_back(condition->getLocation(), msg);
    return;
  }
}

void SemanticAnalyzer::visit(WhileNode &p_while) {
  p_while.visitChildNodes(*this);

  auto *condition = p_while.getCondition();

  if (condition->getReturnType() == nullptr) {
    return;
  }

  if (!condition->getReturnType()->isBool()) {
    std::string msg = "the expression of condition must be boolean type";
    errors.emplace_back(condition->getLocation(), msg);
    return;
  }
}

void SemanticAnalyzer::visit(ForNode &p_for) {
  symbol_manager.enterScope();

  current_symbol_kind = SymbolEntry::SymbolKind::kLoopVar;
  is_in_loop_body = false;
  p_for.visitLoopVar(*this);

  current_symbol_kind = SymbolEntry::SymbolKind::kVariable;
  is_in_loop_body = true;
  p_for.visitBody(*this);
  is_in_loop_body = false;

  if (dump_table) symbol_manager.dumpTopTable();
  symbol_manager.exitScope();

  auto *initial_value = p_for.getInitialValue();
  auto *final_value = p_for.getFinalValue();

  if (initial_value->getReturnType() == nullptr ||
      final_value->getReturnType() == nullptr) {
    return;
  }

  if (initial_value->getConstant()->getValue().integer >
      final_value->getConstant()->getValue().integer) {
    std::string msg =
        "the lower bound and upper bound of iteration count must be in the "
        "incremental order";
    errors.emplace_back(p_for.getLocation(), msg);
    return;
  }
}

void SemanticAnalyzer::visit(ReturnNode &p_return) {
  p_return.visitChildNodes(*this);
  current_symbol_kind = SymbolEntry::SymbolKind::kVariable;

  if (context_manager.top()->getKind() == SymbolEntry::SymbolKind::kProgram ||
      context_manager.top()->getFunctionAttr()->getReturnType()->isVoid()) {
    std::string msg = "program/procedure should not return a value";
    errors.emplace_back(p_return.getLocation(), msg);
    return;
  }

  auto *ret_val = p_return.getRetVal();

  if (ret_val->getReturnType() == nullptr) {
    return;
  }

  auto *func_return_type =
      context_manager.top()->getFunctionAttr()->getReturnType().get();

  if (*ret_val->getReturnType() != *func_return_type) {
    std::string msg = "return '" +
                      std::string(ret_val->getReturnType()->getPTypeCString()) +
                      "' from a function with return type '" +
                      std::string(func_return_type->getPTypeCString()) + "'";
    errors.emplace_back(ret_val->getLocation(), msg);
    return;
  }
}
