# hw4 report

| Field | Value   |
| ----: | :------ |
|  Name | 邱俊耀  |
|    ID | 0710734 |

## How much time did you spend on this project

About 15 hours.

## Project overview

This is a semantic analyzer for the `P` language implemented by `bison`.

### Symbol Table

To implement the symbol table mechanism, I made three classes in `sema/SymbolTable.hpp`:

1. `SymbolEntry`:
    * Presents a single entry in the symbol table.
    * Manages `Attribute` by `union`.

        ```cpp
        union Attribute {
            const Constant *constant;
            const FunctionNode *function;
        };
        ```

    * Uses `bool decl_error` to record if it declared with error.

2. `SymbolTable`:
    * Presents the symbol table.
    * Has a list of `SymbolEntry`s.
    * Provides `lookup` to check if some symbol is in this symbol table and `addSymbol` to add a new symbol into this symbol table.

3. `SymbolManager`:
    * Has a list of `SymbolTable`s.
    * Provides `lookup` to check if some symbol is in all the symbol tables and `addSymbol` to add a new symbol into the top symbol table.
    * Provides `enterScope` and `exitScope` to add/remove a symbol table and increase/decrease current scope level.

### Semantic Error Messages

Used a struct `ErrorMessage` to record the error messages.

```cpp
struct ErrorMessage {
  Location location;
  std::string msg;
};
```

### Semantic Analyzer

To perform semantic analyze, a semantic analyzer has a `SymbolManager` and a list of `ErrorMessage`s (`std::vector<ErrorMessage> errors`).

#### `SymbolManager`

* When `visit` a `VariableNode` or a `FunctionNode`, invoked `lookup` to check if the symbol was redeclared, then invoked `addSymbol` to add a new symbol to current symbol table.
* For the other `AstNode`s, invoked `lookup` to retrieve the components of itself if needed.

#### Dump `ErrorMessage`s

Invoked `dumpErrors(const char *src_path)` in `parser.y` which will parse the source file to lines and dump each `ErrorMessage` in `errors`.

```cpp
// parser.y
if (sema_analyzer.hasErrors()) {
  sema_analyzer.dumpErrors(argv[1]);
} else {
  printf(
      "\n"
      "|---------------------------------------------------|\n"
      "|  There is no syntactic error and semantic error!  |\n"
      "|---------------------------------------------------|\n");
}
```

## What is the hardest you think in this project

1. How to determine the return type of each `ExpressionNode`s?

    For every `AstNode`s that needed to do type checking, add getters to get their children `ExpressionNode`s. Also, add `getReturnType()` and `setReturnType(PType *p_type)` into `ExpressionNode`.
    * When `visit` a `ExpressionNode`s, we needed to set its return type.
    * When `visit` a node that needed to do type checking, we can retrieve each child `ExpressionNode`'s return type then perform type checking.

2. How to determine current context for `return`?

    The `SemanticAnalyzer` maintained a `std::stack<SymbolEntry *> context_manager` which contained a Program `SymbolEntry` and Function `SymbolEntry`s.
    * When `visit` a `ProgramNode` or a `FunctionNode`, I pushed the `SymbolEntry` of the node into the stack before invoked the node's `visitChildNodes()`, then popped the `SymbolEntry` after that.
    * When `visit` a `ReturnNode`, retrieved the top `SymbolEntry` from the stack and check if its return type.

## Feedback to T.A.s

During the end of this semester, there is a lot of work to do for me. Thanks for extending the due date of this homework.
I think I didn't well plan how to implement the homework. Thus, the code is a little messy. I'm looking forward to for the release of reference answer in next homework.
