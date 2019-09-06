#include <nlohmann/json.hpp>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <optional>
#include "bril-ir.hpp"
#include "bril-ir-printer.hpp"
#include "bril-ir-detail.hpp"

using namespace nlohmann;
using namespace std;
using namespace bril_ir;



using block = std::list<instruction>;

int main(){

  std::list<block> accum;
  std::map<const string, block> label_map;
  json j;  std::cin >> j;
  program p{j};
  std::cout << p << std::endl;
  
}
