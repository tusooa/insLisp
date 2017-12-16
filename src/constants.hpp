#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "struct.hpp"
namespace Lisp
{
  const Values::Symbol argvName("ARGV");
  const Values::Symbol optRest("&rest");
  const Values::Symbol optOptional("&optional");
  //const Values::Symbol lambdaDefaultArgs("-args-");
}
#endif
