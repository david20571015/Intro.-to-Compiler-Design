#include "sema/SymbolTable.hpp"

#include <stdio.h>

#include <algorithm>

// SymbolEntry

const char *kSymbolKindString[] = {"program",  "function", "parameter",
                                   "variable", "loop_var", "constant"};

void SymbolEntry::dump() const {
  printf("%-33s", name.id.c_str());
  printf("%-11s", kSymbolKindString[static_cast<uint8_t>(kind)]);
  printf("%d%-10s", level, level ? "(local)" : "(global)");
  printf("%-17s", type->getPTypeCString());

  switch (kind) {
    case SymbolKind::kConstant:
      printf("%-11s", attr.constant->getConstantValueCString());
      break;
    case SymbolKind::kFunction:
      printf("%-11s", attr.function->getParametersTypeCString());
      break;
    default:
      printf("%-11s", "");
      break;
  }

  puts("");
}

// SymbolTable

std::optional<SymbolEntry> SymbolTable::lookup(const std::string &str) {
  auto target =
      std::find_if(entries.begin(), entries.end(),
                   [&str](auto &entry) { return entry.isEqual(str); });

  if (target == entries.end()) {
    return std::nullopt;
  } else {
    return *target;
  }
}

void SymbolTable::dump(void) {
  dumpDemarcation('=');
  printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type",
         "Attribute");
  dumpDemarcation('-');

  auto dump_entry = [](auto &symbol_entry) { symbol_entry.dump(); };
  for_each(entries.begin(), entries.end(), dump_entry);

  dumpDemarcation('-');
}

void SymbolTable::dumpDemarcation(const char chr) {
  for (size_t i = 0; i < 110; ++i) {
    printf("%c", chr);
  }
  puts("");
}

// SymbolManager

void SymbolManager::enterScope() {
  SymbolTableSharedPtr new_scope = std::make_shared<SymbolTable>(level++);
  tables.push_back(new_scope);
}

void SymbolManager::exitScope() {
  tables.pop_back();
  level--;
}

std::optional<SymbolEntry> SymbolManager::lookup(const std::string &str) {
  for (auto table_iter = tables.rbegin(); table_iter != tables.rend();
       ++table_iter) {
    auto entry = table_iter->get()->lookup(str);
    if (entry) {
      return entry;
    }
  }
  return std::nullopt;
}
