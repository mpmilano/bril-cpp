#pragma once
#include "bril-ir.hpp"
#include "bril-ir-parser.hpp"

namespace bril_ir{
  program::program(const nlohmann::json& j):instrs(parse_json(j)){}
  
  namespace instructions{
#define __bril_instructions_boilerplate_impl(x,y...)			\
    y void x ::visit(ir_visitor& v) const {v.visit(*this);}		\
    y _instruction* x ::clone() const {return new std::decay_t<decltype(*this)>{*this}; }
    
    __bril_instructions_boilerplate_impl(print)
    __bril_instructions_boilerplate_impl(_const<int>, template<>)
  }
}
