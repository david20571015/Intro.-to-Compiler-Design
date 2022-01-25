#include "AST/function.hpp"

#include <algorithm>

#include "AST/decl.hpp"

const char *FunctionNode::getParametersTypeCString() const {
  if (!m_parameter_string_is_valid) {
    m_parameter_string = "";

    for (const auto &parameter : m_parameters) {
      for (const auto &var_node : parameter->getVariables()) {
        m_parameter_string.append(var_node->getTypeCString()).append(", ");
      }
    }

    if (!m_parameters.empty()) {
      // remove trailing ", "
      m_parameter_string.erase(m_parameter_string.end() - 2,
                               m_parameter_string.end());
    }

    m_parameter_string_is_valid = true;
  }

  return m_parameter_string.c_str();
}

const char *FunctionNode::getPrototypeCString() const {
  if (!m_prototype_string_is_valid) {
    m_prototype_string = m_ret_type->getPTypeCString();

    m_prototype_string += " (";
    m_prototype_string += getParametersTypeCString();
    m_prototype_string += ")";

    m_prototype_string_is_valid = true;
  }

  return m_prototype_string.c_str();
}

void FunctionNode::visitChildNodes(AstNodeVisitor &p_visitor) {
  auto visit_ast_node = [&](auto &ast_node) { ast_node->accept(p_visitor); };

  for_each(m_parameters.begin(), m_parameters.end(), visit_ast_node);

  if (m_body) {
    visit_ast_node(m_body);
  }
}

void FunctionNode::visitParameters(AstNodeVisitor &p_visitor) {
  auto visit_ast_node = [&](auto &ast_node) { ast_node->accept(p_visitor); };
  for_each(m_parameters.begin(), m_parameters.end(), visit_ast_node);
}

void FunctionNode::visitBody(AstNodeVisitor &p_visitor) {
  if (m_body) {
    m_body->visitChildNodes(p_visitor);
  }
}
