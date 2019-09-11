#pragma once
/*#include <nlohmann/json.hpp>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <optional>
#include <tuple>
#include "copy_ptr.hpp"
#include "const_string.hpp"//*/

namespace bril_ir{
  
  namespace instructions{
    template<typename Base_type, typename... Args>
    struct __general_op{
      const std::tuple<reg<Args>...> args_tpl;
      virtual ~__general_op() = default;
      std::list<reg<Any> > args() const {
	return
	  std::apply([](auto ...x){
		       return std::list<reg<Any> >{reg<Any>{x}...};},
	    args_tpl);
      }
      
      virtual const Base_type& down() const = 0;
      
      template<std::size_t s>
      const auto& arg() const {
	static_assert(s < sizeof...(Args), "Error: this op does not have that many args");
	return std::get<s>(args_tpl);
      }
    protected:
      __general_op(const __general_op&) = default;
      __general_op(const reg<Args>&... args):args_tpl(args...){}
    };
    
    template<typename Base_type, typename... Args>
    struct __effect_op : public effect_op, private __general_op<Base_type, Args...>{
      virtual ~__effect_op() = default;
      
      std::list<reg<Any> > args() const {
	return __general_op<Base_type, Args...>::args();
      }
      
      template<std::size_t s> decltype(auto) arg() const {
	return __general_op<Base_type, Args...>::template arg();
      }
      
    protected:
      __effect_op(const __effect_op&) = default;
      __effect_op() = default;
      __effect_op(const reg<Args>&... args):__general_op<Base_type, Args...>(args...) {};
    };
    
    template<typename Base_type, typename T, typename... Args>
    struct __value_op : public value_op<T>, private __general_op<Base_type, Args...>{
      virtual ~__value_op() = default;
      
      std::list<reg<Any> > args() const {
	return __general_op<Base_type, Args...>::args();
      }
      
      template<std::size_t s> decltype(auto) arg() const {
	return __general_op<Base_type, Args...>::template arg();
      }
      
    protected:
      __value_op(const __value_op&) = default;
      __value_op() = default;
      __value_op(const reg<T> &ret, const reg<Args>&... args):
	value_op<T>(ret),
	__general_op<Base_type, Args...>(args...) {};
    };
    
#define EFFECT_OP(print, Any...)					\
    struct print : public __effect_op<print, Any>{			\
      template<typename... Args>					\
      print(const Args& ... args):__effect_op<print, Any>(args...){}	\
      									\
      ___bril_instructions_boilerplate_decl(print)			\
    };
    
#define VALUE_OP(Name, ret, args...)					\
    struct Name : public __value_op<Name,ret,args> {			\
      template<typename... Args>					\
      Name(const Args& ... _args):__value_op<Name,ret,args>(_args...){}	\
      ___bril_instructions_boilerplate_decl(Name)			\
    };

#define VISIT(x) virtual void visit(const instructions:: x &){};
  }
}
