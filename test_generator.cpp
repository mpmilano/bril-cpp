#include "generator.hpp"
#include <iostream>

using namespace mutils;

int main(){
  generate_generator_t<int ()> generate_generator =
    [](yielder<int> yield){
      return [yield]() mutable {
	       for (int i = 0; ; ++i) yield(i);
	     };
    };
  auto gen = make_generator<int>(generate_generator);
  while (true) std::cout << gen() << std::endl;
  return 0;
}
