#ifndef FUNC_HPP
#define FUNC_HPP

#include "struct.hpp"
namespace Lisp
{
  using namespace Lisp::Values;
  const Hash BuiltinFunc =
  {
    {Symbol("quote"), Any(Func([](EnvPtr, List l)
    {
      if (l.size() == 1) {
        return Any(l[0]);
      } else {
        throw std::invalid_argument("wrong-number-arguments");
      }
    }).quote()) },
    {Symbol("q"), Any(Func([](EnvPtr, List l)
    {
      if (l.size() == 1) {
        return Any(l[0]);
      } else {
        throw std::invalid_argument("wrong-number-arguments");
      }
    }).quote()) },
    {Symbol("+"), Any(Func([](EnvPtr, List l)
     {
       Number result;
       for (auto && i : l) {
         result += i.num();
       }
       return Any(result);
     }))},
    {Symbol("*"), Any(Func([](EnvPtr, List l)
     {
       Number result = 1;
       for (auto && i : l) {
         result *= i.num();
       }
       return Any(result);
     }))},
    {Symbol("if"), Any(Func([](EnvPtr e, List l) -> Any
    {
      if (l.size() <= 1) {
        throw std::invalid_argument("wrong-number-arguments");
      }
      //if ()
      return Any();
    }).quote())},
  };
}

#endif
