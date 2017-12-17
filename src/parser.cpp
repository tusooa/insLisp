#include "parser.hpp"
#include <sstream>
#include "constants.hpp"
#include "func.hpp"

namespace Lisp
{
  Parser::Parser()
  {
    mesc = defaultEsc;
    genRegex();
  }

#ifdef _USE_BOOST
#define _R_MOD , boost::regex_constants::no_mod_m
#endif

  void Parser::genRegex()
  {
    // find- regex
    mregex = {
      {"command-s", Regex(R"r(^((?:.|\n)*?)(``|$))r" _R_MOD)},
      {"command-e", Regex(R"r(^[\s\n]*(?:''|$))r" _R_MOD)},
      {"num", Regex(R"foo(^[\s\n]*([+-]?(?:(?:\d+[,_\d]*)(?:\.[,_\d]*)?|\.(?:\d+[,_\d]*)))(?=[\s\n(){}]|''|$))foo" _R_MOD)},
      {"symbol", Regex(R"foo(^[\s\n]*([^\s\n(){}`'\\_]+))foo" _R_MOD)},
      {"string-s", Regex(R"foo(^[\s\n]*\{)foo" _R_MOD)},
      {"string-s-d", Regex(R"foo(^(\{))foo" _R_MOD)},
      {"string-e-d", Regex(R"foo(^(\}))foo" _R_MOD)},
      {"string-nospec", Regex(R"foo(^([^{}\\]+))foo" _R_MOD)},
      {"string-esc", Regex(R"foo(^\\(.))foo" _R_MOD)},
      {"paren-s", Regex(R"foo(^[\s\n]*\()foo" _R_MOD)},
      {"paren-e", Regex(R"foo(^[\s\n]*\))foo" _R_MOD)},
    };
  }

  /*template<typename T> Values::List handler(T v)
  {
    Values::List l;
    l.push_back(Any(v));
    return l;
    }*/

  Values::Lambda Parser::parse(Values::String text)
  {
    Values::List l = parseCommand(text);
    Values::List declaration;
    declaration.push_back(optRest);
    declaration.push_back(argvName);
    l.insert(l.begin(), declaration);
    return Values::Lambda(l);
  }
  
  Values::List Parser::parseCommand(Values::String & text, int depth, State_T state)
  {
    Values::List tree;
    while (text.length()) {
      smatch m;
      if (state == literal) {
        if (regex_search(text, mregex["command-s"], m)) {
          if (m.length(1)) {
            tree.push_back(m.str(1));
          }
          if (m.length(2)) {
            state = command;
          }
          text.erase(0, m.length());
        } else {
          // 不应该到这里来
          throw std::invalid_argument("");
        }
      } else if (state == command) {
        if (regex_search(text, mregex["command-e"], m)) {
          state = literal;
          text.erase(0, m.length());
        } else if (regex_search(text, mregex["num"], m)) {
          debug("number: " << m.str(1));
          tree.push_back(numHandler(m.str(1)));
          text.erase(0, m.length());
        } else if (regex_search(text, mregex["string-s"], m)) {
          text.erase(0, m.length());
          Values::String str = parseStr(text, depth);
          debug("string: " << str);
          tree.push_back(str);
        } else if (regex_search(text, mregex["symbol"], m)) {
          debug("symbol: " << m.str(1));
          tree.push_back(Values::Symbol(m.str(1)));
          text.erase(0, m.length());
        } else if (regex_search(text, mregex["paren-s"], m)) {
          text.erase(0, m.length());
          Values::List l = parseCommand(text, depth + 1, state);
          tree.push_back(l);
        } else if (regex_search(text, mregex["paren-e"], m)) {
          text.erase(0, m.length());
          if (depth <= 0) {
            throw std::invalid_argument("right parens outnumber left parens");
          }
          return tree;
        } else {
          // 不应该执行到这儿
          throw std::invalid_argument("");
        }
      } else {
        // 不应该执行到这儿
        throw std::invalid_argument("");
      }
    }
    return tree;
  }


  Values::Number Parser::numHandler(const Values::String & text)
  {
    std::istringstream is(text);
    Values::Number num;
    is >> num;
    return num;
  }
  Values::String Parser::parseStr(Values::String & text, int outerDepth)
  {
    Values::String result;
    int depth = 0;
    while (text.length()) {
      smatch m;
      if (regex_search(text, mregex["string-nospec"], m)) {
        result += m.str(1);
        text.erase(0, m.length());
      } else if (regex_search(text, mregex["string-esc"], m)) {
        debug("escaped: `" << m.str(1) << "'");
        if (mesc.find(m.str(1)) != mesc.end()) {
          result += mesc[m.str(1)];
        } else {
          result += m.str(1);
        }
        text.erase(0, m.length());
      } else if (regex_search(text, mregex["string-e-d"], m)) {
        if (depth <= 0) {
          // 串结束
          text.erase(0, m.length());
          break;
        }
        depth -= 1;
        result += m.str(1);
        text.erase(0, m.length());
      } else if (regex_search(text, mregex["string-s-d"], m)) {
        result += m.str(1);
        text.erase(0, m.length());
        depth += 1;
      } else {
        // 不应该执行到这里
        throw std::invalid_argument("");
      }
    }
        
    return result;
  }
}
