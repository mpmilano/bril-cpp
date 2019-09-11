#pragma once

#include <string>
#include <ostream>

namespace mutils {
  struct const_string{
    const std::string s;
    const_string(const std::string& s):s(s){}
    
    operator std::string() const {
      return s;
    }

    bool operator==(const const_string& s) const {
      return this->s == s.s;
    }
    
    bool operator==(const std::string& s) const {
      return this->s == s;
    }
  };

  std::ostream& operator<<(std::ostream& out, const const_string& s){
    return out << s.s;
  }
  
}


