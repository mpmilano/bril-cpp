#pragma once
#include "bril-ir.hpp"

namespace bril_ir{
  std::ostream& operator<<(std::ostream& in, const operation& op){
    switch (op){
    case operation::_const: return in << "const";
    case operation::_print: return in << "print";
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& in, const type& op){
    switch (op){
    case type::_int: return in << "int";
    case type::_bool: return in << "bool";
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& in, const instruction& i){
    if (i.dst) in << *i.dst << " = ";
    in << i.op;
    if (i.args.size() >= 0 || i.value) {
      in << "(";
      bool first = true;
      for (const auto& arg : i.args){
	if (!first) in << ",";
	else first = false;
	in << arg;
      }
      if (i.value) in << *i.value;
      in << ")";
    }
    if (i.t) in << " : " << *i.t;
    return in;
  }

  std::ostream& operator<<(std::ostream& o, const program& p){
    for (const auto &i : p.instrs){
      o << i << ";" << '\n';
    }
    return o;
  }  
}
