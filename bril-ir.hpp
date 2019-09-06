#pragma once
#include <nlohmann/json.hpp>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <optional>
#include "copy_ptr.hpp"

namespace bril_ir{
  
  enum class operation{
    _const, _print
  };
  
  enum class type{
    _int, _bool
  };
  
  struct ir_visitor;
  
  struct _instruction{
    virtual void visit(ir_visitor&) const = 0;
    virtual _instruction* clone() const = 0;
    virtual ~_instruction() = default;
  };

  namespace instructions{
#define ___bril_instructions_boilerplate_decl void visit(ir_visitor& v) const; \
    _instruction* clone() const;

    template<typename T>
    struct _const : public _instruction{
      const std::string dst;
      const T value;
      _const(const _const&) = default;
      _const(const std::string &dst, const T& value):dst(dst),value(value){}

      ___bril_instructions_boilerplate_decl
    };    

    struct print : public _instruction{
      const std::list<std::string> args;

      print(const print&) = default;
      print(const std::list<std::string>& args):args(args){}
      ___bril_instructions_boilerplate_decl
    };
    
  }

  struct ir_visitor{
    virtual void visit(const instructions::_const<int>&) = 0;
    virtual void visit(const instructions::print&) = 0;
  };

  using instruction = mutils::copy_ptr<_instruction>;

struct program{
  const std::list<instruction> instrs;
  program(const nlohmann::json& j);
};


}
