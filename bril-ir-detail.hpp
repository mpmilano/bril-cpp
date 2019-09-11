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
    __bril_instructions_boilerplate_impl(_const<bool>, template<>)
    __bril_instructions_boilerplate_impl(add)
    __bril_instructions_boilerplate_impl(mul)
    __bril_instructions_boilerplate_impl(sub)
    __bril_instructions_boilerplate_impl(div)
    __bril_instructions_boilerplate_impl(eq)
    __bril_instructions_boilerplate_impl(lt)
    __bril_instructions_boilerplate_impl(gt)
    __bril_instructions_boilerplate_impl(le)
    __bril_instructions_boilerplate_impl(ge)
    __bril_instructions_boilerplate_impl(_not)
    __bril_instructions_boilerplate_impl(_and)
    __bril_instructions_boilerplate_impl(_or)
  }
}
