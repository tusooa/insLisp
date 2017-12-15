#include "parser.hpp"
#include <sstream>

namespace Lisp
{
  Parser::Parser()
  {
    /*if (mdelim.command.empty()) {
      mdelim.command = defaultDelim.command;
    }
    if (mdelim.string.empty()) {
      mdelim.string = defaultDelim.string;
    }
    if (mdelim.paren.empty()) {
      mdelim.paren = defaultDelim.paren;
    }
    const std::vector<Values::String> v = {"escape", "ws", "wsornot", "purenum", "notspecial"};
    for (auto key : v) {
      if (miden.find(key) == miden.end()) {
        miden[key] = defaultIden[key];
      }
      }*/
    genRegex();
  }

  void Parser::genRegex()
  {
    // find- regex
    mregex = {
      {"command-s", Regex(R"r(^((?:.|\n)*?)(``|$))r")},
      {"command-e", Regex(R"r(^[\s\n]*(?:''|$))r")},
      {"num", Regex(R"foo(^[\s\n]*([+-]?(?:(?:\d+[,_\d]*)(?:\.[,_\d]*)?|\.(?:\d+[,_\d]*)))(?=[\s\n(){}]|''))foo")},
      {"symbol", Regex(R"foo(^[\s\n]*([^\s\n(){}`'\\_]+))foo")},
      {"string-s", Regex(R"foo(^[\s\n]*\{)foo")},
      {"string-e", Regex(R"foo(\})foo")},
      {"paren-s", Regex(R"foo(^[\s\n]*\()foo")},
      {"paren-e", Regex(R"foo(^[\s\n]*\))foo")},
    };
  }

  /*template<typename T> Values::List handler(T v)
  {
    Values::List l;
    l.push_back(Any(v));
    return l;
    }*/

  
  
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
          debug("string: ");
          parseStr(text, depth);
          text.erase(0, m.length());
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
  Values::String Parser::parseStr(Values::String & text, int depth)
  {
    return "";
  }
}
