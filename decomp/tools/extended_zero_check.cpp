#include <cmath>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <utility>
#include <cassert>
#include "../src/extended.h"

int main(){
 using E=Extended;
 auto check=[](E value,bool negative){assert(value.m==0);assert(std::signbit(value.asFloat())==negative);assert(std::signbit(value.asDouble())==negative);};
 check(E::from(-0.0f),true);check(E::from(-0.0),true);
 for(bool na:{false,true})for(bool nb:{false,true}){
  auto a=E::from(na?-0.0f:0.0f),b=E::from(nb?-0.0f:0.0f);
  check(a+b,na&&nb);check(a-b,na&&!nb);check(a*b,na!=nb);
  check(a/E::from(nb?-2.0f:2.0f),na!=nb);
 }
 check(E::from(-1.0f)+E::from(1.0f),false);
 check(E::from(1.0f)-E::from(1.0f),false);
 check(E::from(-1.0f)-E::from(-1.0f),false);
}
