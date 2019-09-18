#pragma once
#include <mutex>
#include <utility>
#include <condition_variable>

namespace mutils {
  template<typename T>
  struct generator_impl{

      std::unique_ptr<T> payload;
      std::mutex lock;
      std::condition_variable ret_ready;
      bool can_resume = false;
      std::condition_variable resume_ready;
    
      struct receiver{
	  generator_impl& parent;
	  using lock_t = std::unique_lock<std::mutex>;
	  std::unique_ptr<lock_t> lock_p;
	  
	  std::unique_ptr<T> output(){
	      if (!lock_p) lock_p.reset(new lock_t(parent.lock));
	      if (!payload) {
		  ready.wait(*lock_p, [&]() -> bool {return payload;});
	      }
	      auto ret = std::move(parent.payload);
	      return ret;
	  }
	  
	  void resume_routine(){
	      can_resume = true;
	      lock_p.reset();
	      resume_ready.notify();
	  }
      };

    struct routine{
	generator_impl& parent;
	using lock_t = std::unique_lock<std::mutex>;
	std::unique_ptr<lock_t> lock_p;

	void set_value(T t){
	    assert(lock_p);
	    parent.payload = t;
	    parent.can_resume = false;

	    lock_p.reset();
	    parent.ret_ready.notify();
	}

	void resume(){
	    lock_p.reset(new lock_t());
	    parent.resume_ready.wait([]() -> bool {return parent.can_resume;});
	}
    };
  };
    
    template<typename T> struct generator;
    
    template<typename Ret, typename... Args>
    struct generator<Ret (Args...)>{
	std::shared_ptr<generator_impl<Ret> > i{new generator_impl<Ret>{}};
	struct yielder{
	    typename generator_impl<Ret>::routine& yield_here;
	    void operator()(Ret r){
		yield_here.set_value(r);
		yield_here.resume();
	    }
	};
	using constructor = std::function<std::function<void (Args...)> (yielder) >;
	
	generator(constructor c){
	    c(yielder{decltype(*i)::routine{*i}})
	    std::thread t{[generator_impl]{
			      
			  }};
	    
	}
    };


  template<typename T>
  struct yielder{
    typename one_way_channel<T const * const>::sender &send;
    void operator()(const T& t){
      send.send(&t);
    }
    yielder(decltype(send) &send):send(send){}
  };

  
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
