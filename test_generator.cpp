#include "generator.hpp"
#include <iostream>
#include <chrono>

using namespace mutils;
using namespace std;
using namespace chrono;

int main(){
  generate_generator_t<int ()> generate_generator =
    [](yielder<int> yield){
      return [yield]() mutable {
		 for (int i = 0; ; ++i) {
		     this_thread::sleep_for(1s);
		     yield(i);
		     this_thread::sleep_for(1s);
		 }
	     };
    };
  auto gen = make_generator<int>(generate_generator);
  while (true) std::cout << gen() << std::endl;
  return 0;
}
