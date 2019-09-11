#pragma once
#include <nlohmann/json.hpp>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <optional>
#include <tuple>
#include "copy_ptr.hpp"
#include "const_string.hpp"//*/
#include "bril-ir-types.hpp"
#include "bril-ir-support.hpp"

namespace bril_ir{
  namespace instructions{

    EFFECT_OP(print, Any);
  
  VALUE_OP(add,int, int, int);
  VALUE_OP(mul,int, int, int);
  VALUE_OP(sub,int, int, int);
  VALUE_OP(div,int, int, int);
  
  VALUE_OP(eq,bool, int, int);
  VALUE_OP(lt,bool, int, int);
  VALUE_OP(gt,bool, int, int);
  VALUE_OP(le,bool, int, int);
  VALUE_OP(ge,bool, int, int);
  
  VALUE_OP(_not,bool, bool)
  VALUE_OP(_and,bool, bool, bool)
  VALUE_OP(_or,bool, bool, bool)
    
  }

  struct ir_visitor{
    VISIT(print);

    VISIT(_const<bool>);
    VISIT(_const<int>);
    
    VISIT(add);
    VISIT(mul);
    VISIT(sub);
    VISIT(div);
    
    VISIT(eq);
    VISIT(lt);
    VISIT(gt);
    VISIT(le);
    VISIT(ge);
    
    VISIT(_not)
    VISIT(_and)
    VISIT(_or)

  };



struct program{
  const std::list<instruction> instrs;
  program(const nlohmann::json& j);
};


}
