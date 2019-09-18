#pragma once
#include <future>
#include <utility>
#include <condition_variable>

namespace mutils {
  template<typename T>
  struct one_way_channel{

    struct transmission_t{
      T payload;
      std::future<transmission_t> next;
    };

    struct notice{
      std::future<notice> next;
    };
    
    struct receiver{
      using in_t = std::future<transmission_t>;
      using out_t = std::promise<notice>;
      in_t in;
      out_t out;

      T recv(){
	auto&& ret = in.get();
	in = std::move(ret.next);
	out_t old_promise;
	out.swap(old_promise);
	old_promise.set_value(notice{out.get_future()});
	return ret.payload;
      }
    };

    struct sender{
      using in_t = std::future<notice>;
      using out_t = std::promise<transmission_t>;
      out_t out;
      in_t in;

      void send(T t){
	out_t old_promise;
	out.swap(old_promise);
	old_promise.set_value(transmission_t{t,out.get_future()});
	
      }

      sender(one_way_channel::receiver& r){
	r.in = out.get_future();
      }
    };
  };


  template<typename T>
  struct yielder{
    typename one_way_channel<T const * const>::sender &send;
    void operator()(const T& t){
      send.send(&t);
    }
    yielder(decltype(send) &send):send(send){}
  };

  template<typename> struct __generate_generator_t;

  template<typename Ret, typename... Args> struct __generate_generator_t<Ret (Args...)>{
    using type = std::function<std::function<void (Args...)> (yielder<Ret>) >;
  };
  
  template<typename F>
  using generate_generator_t = typename __generate_generator_t<F>::type;

  
  template<typename> struct generator;
  template<typename Ret, typename... Args> struct generator<Ret (Args...)>{
    typename one_way_channel<Ret const * const>::receiver receiver;
    typename one_way_channel<Ret const * const>::sender sender{receiver};
    generate_generator_t<Ret (Args...)> gen;
    const Ret& operator()() {
      std::thread{gen(yielder<Ret>{sender})}.detach();
      return *receiver.recv();
    }
    generator(decltype(gen) gen):gen(gen){}
    generator(const generator&) = delete;
    generator(generator&&) = default;
  };
  
  template<typename Ret, typename... Args>
  generator<Ret (Args...)> make_generator(const std::function<std::function<void (Args...)> (yielder<Ret>)> &gen_generator){
    
    return generator<Ret (Args...)>{gen_generator};
  }

}
