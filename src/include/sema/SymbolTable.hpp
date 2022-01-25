#ifndef SEMA_SYMBOL_TABLE_H
#define SEMA_SYMBOL_TABLE_H

#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "AST/PType.hpp"
#include "AST/ast.hpp"
#include "AST/constant.hpp"
#include "AST/function.hpp"
#include "AST/utils.hpp"

class SymbolTable;

using SymbolTableSharedPtr = std::shared_ptr<SymbolTable>;

class SymbolEntry {
 public:
  union Attribute {
    const Constant *constant;
    const FunctionNode *function;
  };

  enum class SymbolKind : uint8_t {
    kProgram,
    kFunction,
    kParameter,
    kVariable,
    kLoopVar,
    kConstant
  };

  SymbolEntry(const Location location, const char *const name, SymbolKind kind,
              uint8_t level, PTypeSharedPtr type)
      : name(location.line, location.col, name),
        kind(kind),
        level(level),
        type(type) {}

  void setDeclError(bool error) { decl_error = error; }
  void setConstantAttr(const Constant *constant) { attr.constant = constant; }
  void setFunctionAttr(const FunctionNode *function) {
    attr.function = function;
  }

  const std::string getName() const { return name.id; }
  const Location &getLocation() const { return name.location; }
  const SymbolKind getKind() const { return kind; }
  const uint8_t getLevel() const { return level; }
  const PTypeSharedPtr getType() const { return type; }
  const Constant *getConstantAttr() const { return attr.constant; }
  const FunctionNode *getFunctionAttr() const { return attr.function; }

  bool isDeclError() const { return decl_error; }
  bool isEqual(const std::string &str) const { return name.id == str; }
  void dump(void) const;

 private:
  IdInfo name;
  SymbolKind kind;
  uint8_t level;
  PTypeSharedPtr type;
  Attribute attr;

  bool decl_error = false;
};

class SymbolTable {
 public:
  SymbolTable(uint8_t level) : level(level) {}
  ~SymbolTable() = default;

  SymbolEntry &addSymbol(const Location location, const char *const p_name,
                         SymbolEntry::SymbolKind kind, PTypeSharedPtr type) {
    return entries.emplace_back(location, p_name, kind, level, type);
  }

  std::optional<SymbolEntry> lookup(const std::string &str);
  void dump(void);

 private:
  uint8_t level;
  std::vector<SymbolEntry> entries;

  void dumpDemarcation(const char chr);
};

class SymbolManager {
 public:
  SymbolManager() : level(0) {}
  ~SymbolManager() = default;

  void enterScope();
  void exitScope();

  SymbolEntry &addSymbol(const Location location, const char *const p_name,
                         SymbolEntry::SymbolKind kind, PTypeSharedPtr type) {
    return tables.back()->addSymbol(location, p_name, kind, type);
  }

  uint8_t getLevel() const { return level; }
  std::optional<SymbolEntry> lookup(const std::string &str);
  void dumpTopTable() { tables.back()->dump(); }

 private:
  uint8_t level;
  std::vector<SymbolTableSharedPtr> tables;
};

#endif
