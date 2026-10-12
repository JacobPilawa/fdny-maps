#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include "../src/player_fire_exposure.h"

static uint32_t bits(float value){uint32_t result;std::memcpy(&result,&value,sizeof(result));return result;}

int main(){
 try{
  int failures=0;
  auto check=[&](bool value,const char* message){if(!value){std::cerr<<message<<"\n";failures++;}};

  float exposure=.25f;int passouts=0;
  auto below=accumulatePlayerFireExposure(exposure,.5f,.5f,[&]{passouts++;});
  check(bits(exposure)==bits(.55f),"binary32 accumulator store mismatch below threshold");
  check(!below.passoutTriggered&&passouts==0,"below-threshold exposure triggered passout");

  exposure=1.0f;
  auto equal=accumulatePlayerFireExposure(exposure,0.0f,1.0f,[&]{passouts++;});
  check(bits(exposure)==bits(1.0f),"exact threshold store mismatch");
  check(!equal.passoutTriggered&&passouts==0,"strict threshold must not trigger at exactly 1.0");

  // The raw compare observes extended ST(0), even when FST rounds its memory
  // copy back to exactly 1.0f.
  exposure=1.0f;
  const float tiny=std::ldexp(1.0f,-55);
  auto roundsToOne=accumulatePlayerFireExposure(exposure,tiny,1.0f,[&]{
    check(bits(exposure)==bits(1.0f),"passout callback must run after the binary32 store");
    passouts++;
  });
  check(bits(exposure)==bits(1.0f),"extended-over-one sample must round to binary32 1.0");
  check(roundsToOne.passoutTriggered&&passouts==1,"extended value above 1.0 must trigger despite stored 1.0f");

  exposure=.5f;
  auto above=accumulatePlayerFireExposure(exposure,1.0f,.5f,[&]{passouts++;});
  check(bits(exposure)==bits(.5f+1.2f*.5f),"source-ordered exposure result mismatch");
  check(above.passoutTriggered&&passouts==2,"above-threshold exposure did not trigger passout once");

  std::cout<<"Player fire exposure source-derived control flow: failures="<<failures
           <<" (native check only; original 00451640 P-code fixture missing)\n";
  return failures?1:0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 2;}
}
