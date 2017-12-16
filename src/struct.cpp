#include "struct.hpp"
#include <string>
#include <exception>
namespace Lisp
{
  namespace Values
  {
    // Any
    String Any::stringify() const
    {
      String result = "";
      switch (mtype) {
      case Type::symbol:
        result += msym.name();
        break;
      case Type::str:
        result += "''" + mstr + "``";
        break;
      case Type::num:
        result += std::to_string(mnum);
        break;
      case Type::list:
        result += "(";
        for (auto && item : mlist) {
          result += item.stringify() + " ";
        }
        if (result.back() == ' ') {
          result.pop_back();
        }
        result += ")";
        break;
      case Type::func:
        result += "<func>";
        break;
      case Type::lambda:
        result += mlambda.stringify();
        break;
      default:
        // 不应该执行到这里
        break;
      }
      return result;
    }
    // Func
    Func::Func(_Func func, bool quoted) : mfunc(func), mquoted(quoted)
    {
    }
    Any Func::operator()(EnvPtr e, List l)
    {
      if (! mfunc) {
        throw std::invalid_argument("void function");
      }
      return mfunc(e, l);
    }
    // Lambda
    Lambda::Lambda(List exprs, ScopePtr defScope) : mdefScope(defScope)
    {
      Any argList = exprs.front();
      if (argList.type() != Type::list) {
        throw std::invalid_argument("first arg must be a list");
      }
      exprs.erase(exprs.begin());
      margNames = argList.list();
      mexprs = exprs;
    }
    String Lambda::stringify() const
    {
      String result("(lambda ");
      result += Any(margNames).stringify();
      result += " {defScope=" + std::to_string((int)mdefScope.get()) + "} ";
      String lists = Any(mexprs).stringify();
      lists.erase(lists.begin());
      lists.pop_back();
      result += lists;
      result += ")";
      return result;
    }
    void Lambda::pairKV(EnvPtr e)
    {
      // 
    }
    Any Lambda::evalOutOfBox(EnvPtr e, List l)
    {
      pairKV(e);
      return Any();
    }
    Any Lambda::value(EnvPtr e, List l)
    {
      return Any();
    }
  };
  // Scope
  Scope::Scope(ParserPtr parser, ScopePtr parent) : mparser(parser), mparent(parent), readonly(false)
  {
    if (mparent) {
      mdepth = mparent->mdepth + 1;
    }
    else {
      mdepth = 1;
    }
    /*if (mdepth > mparser->maxDepth()) {
      throw std::overflow_error("exceeding max depth");
      }*/
  }
  /*Values::Any Scope::var(const Values::Symbol & s) const
    {

    }*/

  Scope & Scope::makeVar(const Values::Symbol &name)
  {
    mvars[name] = Values::List();
    return *this;
  }// 在本 Scope 里定义变量然后设为空。
}; 
