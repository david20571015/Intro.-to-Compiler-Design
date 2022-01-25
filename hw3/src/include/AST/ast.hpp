#ifndef AST_AST_NODE_H
#define AST_AST_NODE_H

#include <cstdint>
#include <visitor/AstNodeVisitor.hpp>

class AstNodeVisitor;

struct Location {
  uint32_t line;
  uint32_t col;

  Location(const uint32_t line, const uint32_t col) : line(line), col(col) {}
  ~Location() = default;
};

class AstNode {
 public:
  AstNode(const uint32_t line, const uint32_t col);
  virtual ~AstNode() = 0;

  const Location &getLocation() const;

  virtual void accept(AstNodeVisitor &p_visitor) = 0;
  virtual void visitChildNodes(AstNodeVisitor &p_visitor) = 0;

 protected:
  Location location;
};

#endif
