// builtin functions.

#ifndef FUNC_HPP
#define FUNC_HPP

#include <iostream>
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
       Number result=0;
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
	 { Symbol("-"), Any(Func([](EnvPtr, List l)
	 {
		 Number result;
		 if (l.size() == 0) {
			 result = 0;
		 }
		 else {
			 result = l[0].num();
			 if (l.size() == 1) {
				 result = -result;
			 }
			 else {
				 l.erase(l.begin());
				 for (auto && i : l) {
					 result -= i.num();
				 }
			 }
		 }
		 return Any(result);
	 })) },
	 { Symbol("/"), Any(Func([](EnvPtr, List l)
	 {
		 Number result;
		 if (l.size() == 0) {
			 result = 1;
		 }
		 else {
			 result = l[0].num();
			 if (l.size() == 1) {
				 result = 1/result;
			 }
			 else {
				 l.erase(l.begin());
				 for (auto && i : l) {
					 result /= i.num();
				 }
			 }
		 }
		 return Any(result);
	 })) },
	 { Symbol("="), Any(Func([](EnvPtr, List l) -> Any
	 {
		 int a=l.size();
		 if (a == 1) {
			 return Symbol("t");
		 }
		 else {
			 int i;
			 for (i = 0; i < a-1; i++) {
				 if (l[i].num() != l[i + 1].num())
					 return Any();
			 }
			 return Symbol("t");
		 }
	 })) },
	 { Symbol(">"), Any(Func([](EnvPtr, List l) -> Any
	 {
		 int a = l.size();
		 if (a == 1) {
			 return Symbol("t");
		 }
		 else {
			 int i;
			 for (i = 0; i < a - 1; i++) {
				 if (l[i].num()<= l[i + 1].num())
					 return Any();
			 }
			 return Symbol("t");
                 }
	 })) },
	 { Symbol("<"), Any(Func([](EnvPtr, List l) -> Any
	 {
		 int a = l.size();
		 if (a == 1) {
			 return Symbol("t");
		 }
		 else {
			 int i;
			 for (i = 0; i < a - 1; i++) {
				 if (l[i].num()>=l[i + 1].num())
					 return Any();
			 }
			 return Symbol("t");
                 }
	 })) },
	 { Symbol("concat"), Any(Func([](EnvPtr, List l) -> Any
	 {
		 String result = "";
		 for (auto && i : l) {
			 result += i.str();
		 }
		 return Any(result);
	 })) },
	 { Symbol("#"), Any(Func([](EnvPtr, List l)
	 {
		 return Any();
	 }).quote()) },
	 { Symbol("list"), Any(Func([](EnvPtr, List l)
	 {
		 return Any(l);
	 })) },
	 { Symbol("dumper"), Any(Func([](EnvPtr, List l)
	 {
		 return Any(l[0].stringify());
	 })) },
	 { Symbol("length"), Any(Func([](EnvPtr, List l)
	 {
		 return Any(l[0].str().length());
	 })) },
    {Symbol("if"), Any(Func([](EnvPtr e, List l) -> Any
    {
      if (l.size() <= 1) {
        throw std::invalid_argument("wrong-number-arguments");
      }
      if (l[0].value(e).isTrue()) {
        return l[1].value(e);
      } else {
        if (l.size() > 2) {
          Any result;
          for (std::size_t it = 2; it < l.size(); it++) {
            result = l[it].value(e);
          }
          return result;
        }
        return Any();
      }
    }).quote())},
    {Symbol("lambda"), Any(Func([](EnvPtr e, List l)
    {
      return Any(Lambda(l, e->scope()));
    }).quote())},
    {Symbol("progn"), Any(Func([](EnvPtr e, List l)
    {
      if (l.size() < 1) {
        throw std::invalid_argument("wrong number aarguments");
      }
      return l[l.size() - 1];
    }))},
    {Symbol("print"), Any(Func([](EnvPtr e, List l) -> Any
                                 {
                                   for (auto && i : l) {
                                     std::cout << (i.type() == Type::str ? i.str() : i.stringify());
                                   }
                                   return Any();
                                 }))},
    {Symbol("setq"), Any(Func([](EnvPtr e, List l)
    {
      if (l.size() == 0 || l.size() % 2 == 1) {
        throw std::invalid_argument("wrong-number-arguments");
      }
      Any result;
      for (std::size_t i = 0; i < l.size(); i += 2) {
        if (l[i].type() != Type::symbol) {
          throw std::invalid_argument("varname is not a symbol");
        }
        result = l[i + 1].value(e);
        e->var(l[i].sym(), result);
      }
      return result;
    }).quote())},
  };
}

#endif
