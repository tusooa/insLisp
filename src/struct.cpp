#include "struct.hpp"
#include <string>
namespace Lisp
{
  namespace Values
  {
    // Any
    String Any::stringify()
    {
      String result = "";
      switch(mtype) {
      case symbol:
        result += msym.name();
        break;
      case str:
        result += "''" + mstr + "``";
        break;
      case num:
        result += std::to_string(mnum);
        break;
      case list:
        result += "(";
        for (auto && item : mlist) {
          result += item.stringify() + " ";
        }
        if (result.back() == ' ') {
          result.pop_back();
        }
        result += ")";
        break;
      }
      return result;
    }
    // list
    // Lambda
  };
};
