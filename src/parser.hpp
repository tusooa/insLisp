#ifndef PARSER_HPP
#define PARSER_HPP
#include <memory>
#include "struct.hpp"
#include <regex>

namespace Lisp
{
  class Parser
  {
    Hash mvars;
    typedef std::map<Values::String, std::pair<Values::String, Values::String> > Delim_T;
    Delim_T mdelim;
    typedef std::regex Regex;
    typedef std::map<Values::String, Regex> Regexes_T;
    Regexes_T mregex;
  public:
    Parser(Delim_T delim = {}, Regexes_T regex = {}) {}
    virtual ~Parser() {}
  };
};

#endif
