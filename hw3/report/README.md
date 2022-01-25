# hw3 report

| Field | Value   |
| ----: | :------ |
|  Name | 邱俊耀  |
|    ID | 0710734 |

## How much time did you spend on this project

About 10 hours.

## Project overview

This is an AST(abstract syntax tree) generator for the `P` language implemented by `bison`.

### scanner.l

Return value by `yylval` for int, real, boolean and string literal constants.

### parser.y

#### %union

Define these types:

1. The type of literal constants in C++ Fundamental types.

2. Custom type of `P` language.

3. Pointer to `AstNode` and its derived classes.

4. Pointer to `std::vector` of pointer to `AstNode` and its derived classes.

Also, define the type of each non-terminal symbol by %type.

#### Syntax rules

Create each node dynamically by `new` and pass those nodes in production body into the constructor of the production head.

### Derived classes from `AstNode`

The parameter of the constructor are line, col, and pointers to its child nodes. Moreover, use a pointer to a `std::vector` of pointer to node if this node can have any number of child node.

Define functions like `const char* getNameCString()` which return the information of the node. Then invoke them in [AstDumper.cpp](https://github.com/compiler-f21/hw3-david20571015/blob/master/src/lib/AST/AstDumper.cpp).

### Visitor pattern

Each derived class from `AstNode` must implement `void accept(AstNodeVisitor &p_visitor)` and `void visitChildNodes(AstNodeVisitor &p_visitor)`.

#### accept

Simply invoke `p_visitor.visit(*this)` to visit the node. `AstNodeVisitor` can classify different types of the node with its `*this`.

#### visitChildNodes

Invoke `accept` of each child node sequentially.

For example, `ProgramNode` has child nodes `std::vector<DeclNode *> *declarations`, `std::vector<FunctionNode *> *functions` and `CompoundStatementNode *compound_statement`. The implementation of `visitChildNodes` is:

```c++
// program.cpp
void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) {
  for (auto &decl : *declarations) decl->accept(p_visitor);
  for (auto &func : *functions) func->accept(p_visitor);
  compound_statement->accept(p_visitor);
}
```

## What is the hardest you think in this project

How to present **type** and **operator** in `P` language.

My [approach for **type**](https://github.com/compiler-f21/hw3-david20571015/blob/master/src/include/AST/AstType.hpp) is to write a `class DataType` and a `enum ScalarDataType`. And retrieve the type in C-string by `DataType::getTypeCString()`. **Operator** has a similar [approach](https://github.com/compiler-f21/hw3-david20571015/blob/master/src/include/AST/expression.hpp).

## Feedback to T.A.s

This homework is a lot harder than previous two. I spent almost 5 hours to pass the first two unit tests. But after that, the rest unit tests were much easier. I had learned a lot in the homework especially visitor pattern, TAs clearly explained it in the tutorial.
