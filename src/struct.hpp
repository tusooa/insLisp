#ifndef STRUCT_HPP
#define STRUCT_HPP
#include <string>
namespace Lisp
{
  class Env;
  class Scope;
  namespace Values
  {
    enum Type = {symbol, str, num, list, lambda, func};
    class Any;
    class Symbol;
    class List;
    typedef std::string String;
    typedef double Number;
    typedef std::string Symbol;
    typedef std::vector<Any> List;
    typedef Any (* Func)(Env *, List); // builtin func接受两个参数，一个是Env，另一个是 arglist，返回Any。
    class Lambda;

    class Any
    {
      Type mtype;
      Symbol msym;
      List mlist;
      String mstr;
      Number mnum;
      Lambda mlambda;

    public:
      template<typename T> Any(Type type = symbol, const T & val);
      Any();
      virtual ~Any();
      
      Any value(Env * env);
      Any value(Env *, List);
      Any & value(const Any & v);
      Any & value(const Symbol & v);
      Any & value(const String & v);
      Any & value(const List & v);
    };
    class Lambda
    {
      List margNames;
      List mexprs;
      Scope * mdefScope;

    public:
      Lambda(List, Scope *);
      Lambda(const Lambda &);
      virtual ~Lambda();

      Any value(Env *, List);
    };
  }
  class Env
  {
    Scope * mscope, * mdefScope;
    Parser * mparser;
  public:
    Env(Parser * mparser, Scope * scope, Scope * mdefScope = nullptr);
    virtual ~Env();

    Values::Any var(const Values::Symbol &);
    Env & var(const Values::Symbol &, const Values::Any &);
    
  };
  typedef std::map<Values::Symbol, Values::Any> Hash;
  class Scope
  {
    Hash mvars;
    int mref; // 被指向了多少次。事关生命期。
    // 创建时 ref 定义为 0
    // 以下几种情况 ref + 1
    // 有另外的 Scope 指向这个 Scope
    // 有 Lambda 定义在这个 Scope
    // 以下几种情况 ref - 1
    // 子 Scope 被销毁
    // 定义于这个 Scope 里的 Lambda 被销毁
    
    // 一个 Scope 里的代码执行完了，检查ref。
    // ref 是 0 就销毁，不是 0 就保留。
    Scope * mparent;
    Parser * mparser;
    int mdepth;
    bool readonly;
  public:
    Scope(Parser *, Scope * = nullptr);
    Scope(const Scope &);
    virtual ~Scope();

    Values::Any var(const Values::Symbol &); // 存取 vars
    Scope & var(const Values::Symbol &, const Values::Any &);
    Values::Any setVar(const Values::Symbol &, const Values::Any &); // 设定本 Scope 里的变量
    Values::Any getVar(const Values::Symbol &);
    bool hasVarInScope(const Values::Symbol &); // 本 Scope 里是否存在某 var
    Scope * varScope(const Values::Symbol &);
    Scope * varScopeRW(const Values::Symbol &);
    Scope & makeRO();
    Scope & makeRW();
    bool isRO();
    Scope & makeVar(const Values::Symbol &); // 在本 Scope 里定义变量然后设为空。
  };
  
}
#endif
