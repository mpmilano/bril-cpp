#pragma once

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

  using instruction = mutils::copy_ptr<_instruction>;
  
  template<typename>
  using reg = mutils::const_string;

  struct Any{};

  namespace instructions{
#define ___bril_instructions_boilerplate_decl(This) void visit(ir_visitor& v) const; \
    _instruction* clone() const;					\
    const This& down() const { return *this; }
    
    template<typename T>
    struct _const : public _instruction{
      const reg<T> dst;
      const T value;
      _const(const _const&) = default;
      _const(const decltype(dst) &dst, const T& value):dst(dst),value(value){}
      
      ___bril_instructions_boilerplate_decl(_const)
    };

    struct effect_op : public _instruction {
      virtual ~effect_op() = default;
      virtual std::list<reg<Any> > args() const = 0;
    protected:
      effect_op(const effect_op&) = default;
      effect_op() = default;
    };

    template<typename T>
    struct value_op : public _instruction {
      const reg<T> out;
      virtual ~value_op() = default;
      virtual std::list<reg<Any> > args() const = 0;
    protected:
      value_op(const value_op&) = default;
      value_op(const decltype(out)& out):out(out) {};
    };
  }

}
