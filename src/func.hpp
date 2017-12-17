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
	 { Symbol("="), Any(Func([](EnvPtr, List l)
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
	 { Symbol(">"), Any(Func([](EnvPtr, List l)
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
	 })) },
	 { Symbol("<"), Any(Func([](EnvPtr, List l)
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
	 })) },
	 { Symbol("concat"), Any(Func([](EnvPtr, List l)
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
	 })) },
	 { Symbol("list"), Any(Func([](EnvPtr, List l)
	 {
		 return Any(l);
	 })) },
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
