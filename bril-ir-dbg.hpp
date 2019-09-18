#pragma once
#include "const_string.hpp"
#include "copy_ptr.hpp"
#include <iostream>
#include <list>
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <tuple>

namespace bril_ir {

enum class operation { _const, _print };

enum class type { _int, _bool };

struct ir_visitor;

struct _instruction {
  virtual void visit(ir_visitor &) const = 0;
  virtual _instruction *clone() const = 0;
  virtual ~_instruction() = default;
};

using instruction = mutils::copy_ptr<_instruction>;

template <typename> using reg = mutils::const_string;

struct Any {};

namespace instructions {

template <typename T> struct _const : public _instruction {
  const reg<T> dst;
  const T value;
  _const(const _const &) = default;
  _const(const decltype(dst) &dst, const T &value) : dst(dst), value(value) {}

  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const _const &down() const { return *this; }
};

struct effect_op : public _instruction {
  virtual ~effect_op() = 0;
  virtual std::list<reg<Any>> args() const = 0;

protected:
  effect_op(const effect_op &) = default;
  effect_op() = default;
};

template <typename T> struct value_op : public _instruction {
  const reg<T> out;
  virtual ~value_op() = 0;
  virtual std::list<reg<Any>> args() const = 0;

protected:
  value_op(const value_op &) = default;
  value_op(const decltype(out) &out) : out(out){};
};
} // namespace instructions

} // namespace bril_ir

namespace bril_ir {

namespace instructions {
template <typename Base_type, typename... Args> struct __general_op {
  const std::tuple<reg<Args>...> args_tpl;
  virtual ~__general_op() = 0;
  std::list<reg<Any>> args() const {
    return std::apply(
        [](auto... x) { return std::list<reg<Any>>{reg<Any>{x}...}; },
        args_tpl);
  }

  virtual const Base_type &down() const = 0;

  template <std::size_t s> const auto &arg() const {
    static_assert(s < sizeof...(Args),
                  "Error: this op does not have that many args");
    return std::get<s>(args_tpl);
  }

protected:
  __general_op(const __general_op &) = default;
  __general_op(const reg<Args> &... args) : args_tpl(args...) {}
};

template <typename Base_type, typename... Args>
struct __effect_op : public effect_op,
                     private __general_op<Base_type, Args...> {
  virtual ~__effect_op() = 0;

  std::list<reg<Any>> args() const {
    return __general_op<Base_type, Args...>::args();
  }

  template <std::size_t s> decltype(auto) arg() const {
    return __general_op<Base_type, Args...>::template arg();
  }

protected:
  explicit __effect_op(const __effect_op &) = default;
  explicit __effect_op() = default;
  explicit __effect_op(const reg<Args> &... args)
      : __general_op<Base_type, Args...>(args...){};
};

template <typename Base_type, typename T, typename... Args>
struct __value_op : public value_op<T>,
                    private __general_op<Base_type, Args...> {
  virtual ~__value_op() = 0;

  std::list<reg<Any>> args() const {
    return __general_op<Base_type, Args...>::args();
  }

  template <std::size_t s> decltype(auto) arg() const {
    return __general_op<Base_type, Args...>::template arg();
  }

protected:
  __value_op(const __value_op &) = default;
  __value_op() = default;
  __value_op(const reg<T> &ret, const reg<Args> &... args)
      : value_op<T>(ret), __general_op<Base_type, Args...>(args...){};
};

} // namespace instructions
} // namespace bril_ir

namespace bril_ir {
namespace instructions {

struct print : public __effect_op<print, Any> {
  template <typename... Args>
  explicit print(const Args &... args) : __effect_op<print, Any>(args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const print &down() const { return *this; }
};
;

struct add : public __value_op<add, int, int, int> {
  template <typename... Args>
  explicit add(const Args &... _args) : __value_op<add, int, int, int>(_args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const add &down() const { return *this; }
};
;
struct mul : public __value_op<mul, int, int, int> {
  template <typename... Args>
  explicit mul(const Args &... _args) : __value_op<mul, int, int, int>(_args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const mul &down() const { return *this; }
};
;
struct sub : public __value_op<sub, int, int, int> {
  template <typename... Args>
  explicit sub(const Args &... _args) : __value_op<sub, int, int, int>(_args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const sub &down() const { return *this; }
};
;
struct div : public __value_op<div, int, int, int> {
  template <typename... Args>
  explicit div(const Args &... _args) : __value_op<div, int, int, int>(_args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const div &down() const { return *this; }
};
;

struct eq : public __value_op<eq, bool, int, int> {
  template <typename... Args>
  explicit eq(const Args &... _args) : __value_op<eq, bool, int, int>(_args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const eq &down() const { return *this; }
};
;
struct lt : public __value_op<lt, bool, int, int> {
  template <typename... Args>
  explicit lt(const Args &... _args) : __value_op<lt, bool, int, int>(_args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const lt &down() const { return *this; }
};
;
struct gt : public __value_op<gt, bool, int, int> {
  template <typename... Args>
  explicit gt(const Args &... _args) : __value_op<gt, bool, int, int>(_args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const gt &down() const { return *this; }
};
;
struct le : public __value_op<le, bool, int, int> {
  template <typename... Args>
  explicit le(const Args &... _args) : __value_op<le, bool, int, int>(_args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const le &down() const { return *this; }
};
;
struct ge : public __value_op<ge, bool, int, int> {
  template <typename... Args>
  explicit ge(const Args &... _args) : __value_op<ge, bool, int, int>(_args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const ge &down() const { return *this; }
};
;

struct _not : public __value_op<_not, bool, bool> {
  template <typename... Args>
  explicit _not(const Args &... _args) : __value_op<_not, bool, bool>(_args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const _not &down() const { return *this; }
};
struct _and : public __value_op<_and, bool, bool, bool> {
  template <typename... Args>
  explicit _and(const Args &... _args) : __value_op<_and, bool, bool, bool>(_args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const _and &down() const { return *this; }
};
struct _or : public __value_op<_or, bool, bool, bool> {
  template <typename... Args>
  explicit _or(const Args &... _args) : __value_op<_or, bool, bool, bool>(_args...) {}
  void visit(ir_visitor &v) const;
  _instruction *clone() const;
  const _or &down() const { return *this; }
};

} // namespace instructions

#define VISIT(x) virtual void visit(const instructions:: x &i) {}
  
  
  struct ir_visitor {
    VISIT(print)
    VISIT(_const<bool>)
    VISIT(_const<int>)
  VISIT(add)
  VISIT(mul)
  VISIT(sub)
  VISIT(div)
  VISIT(eq)
  VISIT(lt)
  VISIT(gt)
  VISIT(le)
  VISIT(ge)
  VISIT(_not)
  VISIT(_and)
  VISIT(_or)
};

struct program {
  const std::list<instruction> instrs;
  program(const nlohmann::json &j);
};

} // namespace bril_ir
