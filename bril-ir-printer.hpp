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

  std::ostream& operator<<(std::ostream& out, const instruction& i){

    struct visitor : public ir_visitor{
      std::ostream& out;
      
      void visit(const instructions::_const<int>& c){
	out << c.dst << " = " << "const(" << c.value << " : int)";
      }
      
      void visit(const instructions::print& p){
	out << "print(";
	bool first = true;
	for (const auto& arg : p.args()){
	  if (!first) out << ",";
	  else first = false;
	  out << arg;
	}
	out << ")";
      }
      
      void visit(const instruction& i){
	i->visit(*this);
      }

      visitor(std::ostream &out):out(out){}
      
    };
    visitor v{out};
    v.visit(i);
    return out;
  }

  std::ostream& operator<<(std::ostream& o, const program& p){
    for (const auto &i : p.instrs){
      o << i << ";" << '\n';
    }
    return o;
  }  
}
