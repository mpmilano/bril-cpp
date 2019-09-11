#pragma once
#include "bril-ir.hpp"

namespace bril_ir {
  static std::list<instruction> parse_json(const nlohmann::json& j){
    constexpr auto op = [](const std::string& s){
		if (s == "const") return operation::_const;
		if (s == "print") return operation::_print;
		throw "Unhandled";
	      };
    constexpr auto type = [](const std::string& s){
		  if (s == "int") return type::_int;
		  if (s == "bool") return type::_bool;
		  throw "Unhandled";
		};

    constexpr auto parse_print =
      [](auto &ref){
	return instructions::print{std::string{ref["args"][0]}};
      };
    
    constexpr auto parse_const_int =
      [](auto& ref){
	return instructions::_const<int>{std::string{ref["dest"]}, int{ref["value"]}};
      };
    
    std::list<instruction> build;
    for (const auto& instr : j["functions"][0]["instrs"]){
      auto _op = op(instr["op"]);
      switch (_op){
      case operation::_const:
	build.emplace_back(parse_const_int(instr));
	continue;
      case operation::_print:
	build.emplace_back(parse_print(instr));
	continue;
      };
    }
    return build;
  }
}
