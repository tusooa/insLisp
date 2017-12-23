#ifndef STRUCT_HPP
#define STRUCT_HPP
#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <map>

namespace Lisp
{
  class Env;
  class Scope;
  typedef std::shared_ptr<Env> EnvPtr;
  typedef std::shared_ptr<Scope> ScopePtr;
  class Parser;
  typedef std::shared_ptr<Parser> ParserPtr;

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
      bool operator==(const Symbol & another) const { return mname == another.mname; }
      bool operator!=(const Symbol & rhs){ return !(*this == rhs); }
      bool operator<(const Symbol & another) const { return mname < another.mname; }
    };
    typedef std::vector<Any> List;
    class Func;
    class Func
    {
      typedef std::function<Any(EnvPtr, List)> _Func; // builtin func接受两个参数，一个是Env，另一个是 arglist，返回Any。
      _Func mfunc;
      bool mquoted;
    public:
      Func() {}
      Func(_Func func, bool quoted = false); // 这里不能进行实体化，因为用到了_Func (提到了Any)
      bool quoted() { return mquoted; }
      Func & quote(bool q = true) { mquoted = q; return * this; }
      Any operator()(EnvPtr e, List l);
    };

    class Lambda;
    class Lambda
    {
      List margNames;
      List mexprs;
      ScopePtr mdefScope;
      void pairKV(EnvPtr, List);
    public:
      Lambda() {}
      Lambda(List, ScopePtr = nullptr);
      virtual ~Lambda() {}

      String stringify() const;
      Any evalOutOfBox(EnvPtr, List);
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
      Any() : mtype(Type::list) {}
      virtual ~Any() {}
      
      Any value(EnvPtr) const; // 取值
      Type type() const { return mtype; }
      Symbol sym() const { if (mtype == Type::symbol) { return msym; } throw std::invalid_argument("type not match"); }
      List list() const { if (mtype == Type::list) { return mlist; } throw std::invalid_argument("type not match"); }
      String str() const { if (mtype == Type::str) { return mstr; } throw std::invalid_argument("type not match"); }
      Number num() const { if (mtype == Type::num) { return mnum; } throw std::invalid_argument("type not match"); }
      Lambda lambda() const { if (mtype == Type::lambda) { return mlambda; } throw std::invalid_argument("type not match"); }
      Func func() const { if (mtype == Type::func) { return mfunc; } throw std::invalid_argument("type not match"); }
      String stringify() const;
      bool isTrue();
    };
    
  }
  
  class Env
  {
    ScopePtr mscope, mdefScope;
    ParserPtr mparser;
  public:
    Env(ParserPtr parser, ScopePtr scope, ScopePtr defScope = nullptr);
    virtual ~Env() {}

    Values::Any var(const Values::Symbol &);
    Env & var(const Values::Symbol &, const Values::Any &);
    ScopePtr scope() const { return mscope; }
    ScopePtr defScope() const { return mdefScope; }
    ParserPtr parser() const { return mparser; }
  };

  typedef std::map<Values::Symbol, Values::Any> Hash;
  class Scope : public std::enable_shared_from_this<Scope>
  {
    Hash mvars;
    ScopePtr mparent;
    ParserPtr mparser;
    int mdepth;
    bool mreadonly;
  public:
    Scope(ParserPtr, ScopePtr = nullptr);
    //Scope(const Scope &);
    virtual ~Scope() {}

    Values::Any var(const Values::Symbol &); // 存取 vars
    Scope & var(const Values::Symbol &, const Values::Any &);
    Values::Any setVar(const Values::Symbol &, const Values::Any &); // 设定本 Scope 里的变量
    Values::Any getVar(const Values::Symbol &) const;
    bool hasVarInScope(const Values::Symbol &) const; // 本 Scope 里是否存在某 var
    ScopePtr varScope(const Values::Symbol &);
    ScopePtr varScopeRW(const Values::Symbol &);
    Scope & makeRO();
    Scope & makeRW();
    bool isRO() const;
    Scope & makeVar(const Values::Symbol &); // 在本 Scope 里定义变量然后设为空。
  };
  
}

#endif
