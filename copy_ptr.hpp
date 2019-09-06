#pragma once
#include <memory>
#include <type_traits>
#include <cassert>

namespace mutils{
  template<typename T> struct copy_ptr{
    const std::unique_ptr<T> t;
    
    copy_ptr():t(new T()){}
    
    copy_ptr(const copy_ptr& t):t(t->clone()){}

    template<typename U>
    copy_ptr(const copy_ptr<U>& u):t(u->clone()){
      static_assert(std::is_base_of_v<T,U>);
    }

    copy_ptr(const T& t):t(t.clone()){}

    copy_ptr(copy_ptr&&) = default;

    decltype(auto) operator*() const {
      assert(t);
      return *t;
    }

    decltype(auto) operator*() {
      assert(t);
      return *t;
    }

    decltype(auto) operator->(){
      assert(t);
      return t.get();
    }

    decltype(auto) operator->() const {
      assert(t);
      return t.get();
    }
  };
}
