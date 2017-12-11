#include "struct.hpp"
#include <iostream>

using namespace Lisp::Values;

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
  return 0;
}
