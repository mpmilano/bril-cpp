#pragma once
#include <nlohmann/json.hpp>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <optional>
#include "copy_ptr.hpp"

namespace bril_ir{

  struct instruction;
  
  struct ir_visitor{
    virtual visit(const instruction&) = 0;
  };
  
enum class operation{
  _const, _print
};

enum class type{
  _int, _bool
};

union value{
  int i;
  bool b;
};

struct dyn_instruction{
  const std::optional<std::string> dst;
  const std::list<std::string> args;
  const operation op;
  const std::optional<type> t;
  const std::optional<int> value;

  instruction(const std::string& dst, operation op, type t, int value )
    :dst(dst),op(op),t(t),value(value){}
  
  instruction(const std::list<std::string> &args, operation op)
    :args(args),op(op){}
  
  instruction() = default;
};

  struct _instruction{
    virtual visit(ir_visitor&) = 0;
  private:
    ~instruction() = default;
  };

  using instruction = mutils::copy_ptr<_instruction>;

  namespace instructions{
    
    struct _const : public _instruction{
      std::list<std::string> args;
    };
    
    template<typename T>
    struct print : public _instruction{
      std::string dst;
      T value;
    };
    
  }

struct program{
  const std::list<instruction> instrs;
  program(const nlohmann::json& j);
};


}
