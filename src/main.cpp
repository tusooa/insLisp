// the main program.
// runs the lisp code in file if specified a file name
// otherwise runs the repl(read eval print-loop)
#include "struct.hpp"
#include <iostream>
#include "parser.hpp"
#include "func.hpp"
#include <fstream>
#include <sstream>
#include "constants.hpp"

using namespace Lisp::Values;
using namespace Lisp;
int main(int argc, char *argv[])
{
  // test
  ParserPtr il = std::make_shared<Parser>();
  ScopePtr ptr = std::make_shared<Scope>(il); // top scope and top env
  for (auto && kv : BuiltinFunc) { // assign builtin functions
    ptr->var(kv.first, kv.second);
  }
  ptr->var(symT, symT);
  ptr->var(optRest, optRest);
  ptr->var(optOptional, optOptional);
  EnvPtr e = std::make_shared<Env>(il, ptr);
  if (argc <= 1) {
    // repl
    while (true) {
      std::cout << "il> ";
      std::string str;
      std::getline(std::cin, str);
      if (! std::cin) {
        break;
      }
      try {
        Any result = il->parse(str).evalOutOfBox(e, List());
        std::cout << "res= " << result.stringify() << std::endl;
      } catch (std::exception & e) {
        std::cerr << "error:" << e.what() << std::endl;
      }
    }
    return 0;
  }
  std::ifstream f(argv[1]);
  if (! f) {
    std::cerr << "cannot open file " << argv[1] << std::endl;
    return 1;
  }
  std::stringstream strStream;
  strStream << f.rdbuf();
  std::string str = strStream.str(); // whole file is now here
  List argL;
  for (int i = 1; i < argc; i++) {
    argL.push_back(String(argv[i]));
  }
  try {
    il->parse(str).value(e, argL);
  } catch (std::exception & e) {
    std::cerr << "error:" << e.what() << std::endl;
  }
#ifdef DEBUG
  system("pause");
#endif
  return 0;
}
