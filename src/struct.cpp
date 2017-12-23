// structure.
// Any,
// Number, String, Symbol, Func, Lambda, List
#include "struct.hpp"
#include <string>
#include <exception>
#include "constants.hpp"

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
    Any Any::value(EnvPtr e) const
    {
      if (mtype == Type::symbol) {
        return e->var(msym);
      } else if (mtype == Type::list) {
        auto lst = list();
        if (lst.size() < 1) {
          throw std::invalid_argument("empty list");
        }
        Any first = lst[0];
        lst.erase(lst.begin());
        Any firstVal = first.value(e);
        if (firstVal.type() == Type::func) {
          Func f = firstVal.func();
          if (f.quoted()) {
            return f(e, lst);
          } else {
            List valList;
            for (auto && i : lst) {
              valList.push_back(i.value(e));
            }
            return f(e, valList);
          }
        } else if (firstVal.type() == Type::lambda) {
          List valList;
          for (auto && i : lst) {
            valList.push_back(i.value(e));
          }
          return firstVal.lambda().value(e, valList);
        } else {
          throw std::invalid_argument("first item is neither a func nor a lambda");
        }
      } else { // return as-is
        return *this;
      }
    }
    bool Any::isTrue() // 只有nil(空表)是假
    {
      return !(mtype == Type::list && mlist.size() == 0);
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
    void Lambda::pairKV(EnvPtr e, List l)
    {
      bool opt = false, rest = false;
      for (auto && n : margNames) {
        if (n.sym() == optOptional) { // 可选项
          if (rest) {
            throw std::invalid_argument("invalid lambda defination: &optional following &rest");
          }
          opt = true;
          continue; // &optional 本身不作为参数。
        }
        if (n.sym() == optRest) { // 余项
          if (rest) {
            throw std::invalid_argument("invalid lambda defination: &rest following &rest");
          }
          rest = true;
          continue;
        }
        if (rest) {
          List restArgs;
          for (auto && r : l) {
            restArgs.push_back(r);
          }
          e->scope()->setVar(n.sym(), Any(restArgs));
          return;
        }
        if (l.size() == 0) {
          if (opt) {
            e->scope()->setVar(n.sym(), Any()); // default to nil
            continue;
          }
          throw std::invalid_argument("too few arguments");
        }
        e->scope()->setVar(n.sym(), l[0]);
        l.erase(l.begin());
      }
      if (l.size() > 0) {
        throw std::invalid_argument("too many arguments");
      }      
    }
    Any Lambda::evalOutOfBox(EnvPtr e, List l)
    {
      pairKV(e, l);
      Any result;
      for (auto && stat : mexprs) {
        result = stat.value(e);
      }
      return result;
    }
    Any Lambda::value(EnvPtr e, List l)
    {
      // create sub scope and env
      ScopePtr s = std::make_shared<Scope>(e->parser(), e->scope());
      EnvPtr subEnv = std::make_shared<Env>(e->parser(), s, mdefScope);
      return evalOutOfBox(subEnv, l);
    }
  };
  // Scope
  Scope::Scope(ParserPtr parser, ScopePtr parent) : mparser(parser), mparent(parent), mreadonly(false)
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

  Scope & Scope::makeVar(const Values::Symbol &name)
  {
    mvars[name] = Values::List();
    return *this;
  }
  Values::Any Scope::setVar(const Values::Symbol & name, const Values::Any & val)
  {
    return mvars[name] = val;
  }
  Values::Any Scope::getVar(const Values::Symbol & name) const
  {
    Values::Any result = mvars.find(name)->second; // 奇怪的问题。
    return result;
  }
  bool Scope::hasVarInScope(const Values::Symbol & name) const
  {
    return mvars.find(name) != mvars.end();
  }
  Scope & Scope::makeRO()
  {
    mreadonly = true;
    return *this;
  }
  Scope & Scope::makeRW()
  {
    mreadonly = false;
    return *this;
  }
  bool Scope::isRO() const
  {
    return mreadonly;
  }
  // 查找逻辑
  // 先在自己找，如果有，返回自己，否则，递归查找上一级scope。
  // 找不到返回nullptr.
  ScopePtr Scope::varScope(const Values::Symbol & name)
  {
    if (hasVarInScope(name)) {
      return shared_from_this();
    } else if (mparent) {
      return mparent->varScope(name);
    } else {
      return nullptr;
    }
  }
  ScopePtr Scope::varScopeRW(const Values::Symbol & name)
  {
    if (hasVarInScope(name)) {
      if (isRO()) {
        return nullptr;
      } else {
        return shared_from_this();
      }
    } else if (mparent) {
      return mparent->varScopeRW(name);
    } else {
      return nullptr;
    }
  }
  Values::Any Scope::var(const Values::Symbol & name)
  {
    ScopePtr s = varScope(name);
    if (s) {
      return s->getVar(name);
    } else {
      throw std::invalid_argument("no such variable: " + name.name());
    }
  }
  Scope & Scope::var(const Values::Symbol & name, const Values::Any & val)
  {
    ScopePtr s = varScopeRW(name);
    if (s) {
      s->setVar(name, val);
      return *this;
    } else {
      setVar(name, val); // 这里会忽略掉自己的RO属性。
      return *this;
    }
  }

  // Env
  Env::Env(ParserPtr parser, ScopePtr scope, ScopePtr defScope) : mparser(parser), mscope(scope), mdefScope(defScope)
  {
  }
  Values::Any Env::var(const Values::Symbol & name)
  {
    if (! mdefScope) {
      return mscope->var(name);
    }
    if (mscope->hasVarInScope(name)) {
      return mscope->var(name);
    } else {
      ScopePtr s;
      if (s = mdefScope->varScope(name)) {
        return s->var(name);
      } else if (s = mscope->varScope(name)) {
        return s->var(name);
      } else {
        throw std::invalid_argument("variable not found: " + name.name());
      }
    }
  }
  Env & Env::var(const Values::Symbol & name, const Values::Any & val)
  {
    if (! mdefScope) {
      mscope->var(name, val);
      return *this;
    }
    if (mscope->hasVarInScope(name)) {
      mscope->var(name, val);
    } else {
      ScopePtr s;
      if (s = mdefScope->varScopeRW(name)) {
        s->setVar(name, val);
      } else {
        mscope->var(name, val);
      }
    }
    return *this;
  }
};
