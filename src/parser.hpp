#ifndef PARSER_HPP
#define PARSER_HPP
#include <memory>

namespace Lisp
{
  class Parser;
  typedef std::shared_ptr<Parser> ParserPtr;
  class Parser
  {
  public:
    Parser() {}
    virtual ~Parser() {}
  };
}

#endif
