#ifndef PARSER_HPP
#define PARSER_HPP
#include <memory>
#include "struct.hpp"

#ifdef DEBUG
#include <iostream>
#define debug(s) std::cerr << s << std::endl;
#else
#define debug(s) 1
#endif
#ifdef _USE_BOOST // wrapper
#include <boost/regex.hpp>
typedef boost::regex Regex;
using boost::smatch;
//using boost::regex_search;
#define regex_search(A, B, C) boost::regex_search((A), (C), (B))
#else
#include <regex>
typedef std::regex Regex;
using std::smatch;
using std::regex_search;
#endif

namespace Lisp
{
  /*typedef std::vector<std::pair<Values::String, Values::String> > Delims;
  struct Delim_T
  {
    Delims command, string, paren;
  };
  typedef std::map<Values::String, Values::String> Identifiers_T;*/
  typedef std::map<Values::String, Regex> Regexes_T;
  typedef std::map<Values::String, Values::String> Escape_T;
  enum State_T {literal, command};
  /*const Delim_T defaultDelim = {
    { {"``", "''"} }, // command
    { {"\\{", "\\}"} }, // string
    { {"\\(", "\\)"} } // paren
  };
  const Identifiers_T defaultIden = {
    {"escape", R"foo(\\)foo"},
    {"ws", R"foo([\s\n]+)foo"},
    {"wsornot", R"foo([\s\n]*)foo"},
    {"purenum", R"foo(-?(?:(?:\d+[,_\d]*)(?:\.[,_\d]*)?|\.(?:\d+[,_\d]*))(?=\s|\)|''))foo"},
    {"notspecial", R"foo([^\s\n(){}`'\\_]+)foo"}
    };*/
  const Escape_T defaultEsc = {
    {"t", "\t"},
    {"n", "\n"},
    {"\\", "\\"}
  };
  class Parser
  {
    Hash mvars;
    //Delim_T mdelim;
    //Identifiers_T miden;
    Regexes_T mregex;
    Escape_T mesc;
    //Handler_T mhandler;
    void genRegex();
    //template<typename T> Values::List handler(T);
  public:
    Parser();
    virtual ~Parser() {}

    Values::Lambda parse(Values::String);
    Values::List parseCommand(Values::String & text, int depth = 0, State_T state = literal);
    Values::String parseStr(Values::String & text, int outerDepth);
    Values::Number numHandler(const Values::String & text);
    Values::String dd(Values::Any);

  };
};

#endif
