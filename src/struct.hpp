#ifndef STRUCT_HPP
#define STRUCT_HPP
#include <string>
namespace Lisp
{
  class Env;
  class Scope;
  namespace Values
  {
    enum Type = {symbol, str, num, list, lambda};
    class Any;
    class Symbol;
    class List;
    typedef std::string String;
    typedef double Number; // temp
    class Lambda;

    class Any
    {
      Type mtype;
      Symbol msym;
      List mlist;
      String mstr;
      Number mnum;
      bool mquoted;
    public:
      template<typename T> Any(Type type = symbol, const T & val);
      Any();
      virtual ~Any();

      
      Any value(Env * env);
      Any & value(Any v);
    };
    class String : public Any
    {
      std::string mval;
    };
    class Number : public Any
    {
      double mval;
    }
    class Symbol : public Any
    {
      String mname;
    };
    class List : public Any
    {
      std::vector<Any> mval;
    };
  }
  class Env
  {
    Scope * mscope, * mdefScope;
  public:
    Env(Scope * scope, Scope * mdefScope = nullptr);
    
  };
  typedef std::map<std::string, Values::Any> Hash;
  class Scope
  {

  };
}
#endif
