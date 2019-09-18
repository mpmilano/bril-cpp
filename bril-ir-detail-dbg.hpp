#pragma once

#include "bril-ir-parser.hpp"
#include "bril-ir.hpp"

namespace bril_ir {
program::program(const nlohmann::json &j) : instrs(parse_json(j)) {}

namespace instructions {

void print ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *print ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
  
template <> void _const<int>::visit(ir_visitor &v) const { v.visit(*this); }
template <> _instruction *_const<int>::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
template <> void _const<bool>::visit(ir_visitor &v) const { v.visit(*this); }
template <> _instruction *_const<bool>::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
void add ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *add ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
void mul ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *mul ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
void sub ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *sub ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
void div ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *div ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
void eq ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *eq ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
void lt ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *lt ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
void gt ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *gt ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
void le ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *le ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
void ge ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *ge ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
void _not ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *_not ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
void _and ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *_and ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
void _or ::visit(ir_visitor &v) const { v.visit(*this); }
_instruction *_or ::clone() const {
  return new std::decay_t<decltype(*this)>{*this};
}
} // namespace instructions
} // namespace bril_ir
