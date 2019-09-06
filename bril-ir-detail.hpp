#pragma once
#include "bril-ir.hpp"
#include "bril-ir-parser.hpp"

namespace bril_ir{
  program::program(const nlohmann::json& j):instrs(parse_json(j)){}
}
