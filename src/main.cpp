#include "struct.hpp"
#include <iostream>
#include "parser.hpp"

using namespace Lisp::Values;
using namespace Lisp;
int main(int argc, char *argv[])
{
  // test
  List l, d;
  l.push_back(Symbol("foo"));
  l.push_back(String("bar"));
  l.push_back(Number(3));
  d.push_back(Symbol("baz"));
  d.push_back(Number(3.14159));
  l.push_back(d);
  Any p(l);
  std::cout << p.stringify() << std::endl;
  List alist = {Symbol("a"), Symbol("b"), Symbol("c")};
  List flist;
  flist.push_back(alist);
  List l1;
  l1.push_back(Symbol("p"));
  flist.push_back(l1);
  Parser il;
  Scope s(std::make_shared<Parser>(il));
  Lambda lam(flist, std::make_shared<Scope>(s));
  std::cout << lam.stringify() << std::endl;
  std::string str(R"f(mewmew``foo (bar baz) 3.12 )f");
  std::cout << Any(il.parseCommand(str)).stringify() << std::endl;
  return 0;
}
