#ifndef STRUCT_HPP
#define STRUCT_HPP
#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <map>
#include "parser.hpp"

namespace Lisp
{
  class Env;
  class Scope;
  typedef std::shared_ptr<Env> EnvPtr;
  typedef std::shared_ptr<Scope> ScopePtr;
  namespace Values
  {
    enum Type {symbol, str, num, list, lambda, func};
    class Any;
    typedef std::string String;
    typedef double Number;
    class Symbol
    {
      String mname;
    public:
      Symbol(String name = "") : mname(name) {}
      Symbol(const Symbol & s) : mname(s.mname) {}
      virtual ~Symbol() {}
      String name() const { return mname; }
      Symbol & name(String n) { mname = n; return *this; }
    };
    typedef std::vector<Any> List;
    typedef std::function<Any(std::shared_ptr<Env>, List)> Func; // builtin func接受两个参数，一个是Env，另一个是 arglist，返回Any。

    class Lambda;
    class Lambda
    {
      List margNames;
      List mexprs;
      std::shared_ptr<Scope> mdefScope;

    public:
      Lambda() {}
      Lambda(List, ScopePtr = nullptr);
      virtual ~Lambda() {}

      String stringify() const;
      Any value(EnvPtr, List);
    };
    class Any
    {
      Type mtype;
      Symbol msym;
      List mlist;
      String mstr;
      Number mnum;
      Lambda mlambda;
      Func mfunc;

    public:
      //template<typename T> Any(Type type, const T & val);
      Any(const Symbol & v) : mtype(Type::symbol), msym(v) {}
      Any(const List & v) : mtype(Type::list), mlist(v) {}
      Any(const String & v) : mtype(Type::str), mstr(v) {}
      Any(const Number & v) : mtype(Type::num), mnum(v) {}
      Any(const Lambda & v) : mtype(Type::lambda), mlambda(v) {}
      Any(const Func & v) : mtype(Type::func), mfunc(v) {}
      Any();
      virtual ~Any() {}
      
      Any value(EnvPtr) const; // 取值
      Any value(EnvPtr, List) const; // 调用函数
      Type type() const { return mtype; }
      Symbol sym() const { if (mtype == Type::symbol) { return msym; } throw std::invalid_argument("type not match"); }
      List list() const { if (mtype == Type::list) { return mlist; } throw std::invalid_argument("type not match"); }
      String str() const { if (mtype == Type::str) { return mstr; } throw std::invalid_argument("type not match"); }
      Number num() const { if (mtype == Type::num) { return mnum; } throw std::invalid_argument("type not match"); }
      Lambda lambda() const { if (mtype == Type::lambda) { return mlambda; } throw std::invalid_argument("type not match"); }
      Func func() const { if (mtype == Type::func) { return mfunc; } throw std::invalid_argument("type not match"); }
      String stringify() const;
      Any & value(const Any & v);
      Any & value(const Symbol & v);
      Any & value(const String & v);
      Any & value(const List & v);
      Any & value(const Lambda & v);
    };
    
  };
  class Env
  {
    ScopePtr mscope, mdefScope;
    ParserPtr mparser;
  public:
    Env(ParserPtr mparser, ScopePtr scope, ScopePtr mdefScope = nullptr);
    virtual ~Env();

    Values::Any var(const Values::Symbol &) const;
    Env & var(const Values::Symbol &, const Values::Any &);
    
  };
  typedef std::map<Values::Symbol, Values::Any> Hash;
  class Scope
  {
    Hash mvars;
    ScopePtr mparent;
    ParserPtr mparser;
    int mdepth;
    bool readonly;
  public:
    Scope(ParserPtr, ScopePtr = nullptr);
    //Scope(const Scope &);
    virtual ~Scope() {}

    Values::Any var(const Values::Symbol &) const; // 存取 vars
    Scope & var(const Values::Symbol &, const Values::Any &);
    Values::Any setVar(const Values::Symbol &, const Values::Any &); // 设定本 Scope 里的变量
    Values::Any getVar(const Values::Symbol &) const;
    bool hasVarInScope(const Values::Symbol &) const; // 本 Scope 里是否存在某 var
    Scope * varScope(const Values::Symbol &) const;
    Scope * varScopeRW(const Values::Symbol &) const;
    Scope & makeRO();
    Scope & makeRW();
    bool isRO() const;
    Scope & makeVar(const Values::Symbol &); // 在本 Scope 里定义变量然后设为空。
  };
  
};
};

#endif
