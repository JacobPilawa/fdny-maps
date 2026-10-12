#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../src/player_pain_deadline.h"

static void require(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}

int main(){
 try{
  int failures=0;
  auto check=[&](bool condition,const char* message){if(!condition){std::cerr<<message<<"\n";failures++;}};

  Random notDueRandom(0x12345678u);
  uint32_t deadline=5000;
  int enqueued=0;
  check(!updatePlayerPainDeadline(deadline,4999,notDueRandom,[&](Random&){enqueued++;}),"before deadline unexpectedly due");
  check(!updatePlayerPainDeadline(deadline,5000,notDueRandom,[&](Random&){enqueued++;}),"equal deadline unexpectedly due");
  check(deadline==5000&&enqueued==0&&notDueRandom.drawCount==0,"not-due path changed state or drew RNG");

  Random dueRandom(0x12345678u);
  std::vector<char> order;
  dueRandom.observer=[&](uint64_t,uint32_t){order.push_back('R');};
  deadline=5000;
  check(updatePlayerPainDeadline(deadline,5001,dueRandom,[&](Random& shared){
    order.push_back('P');shared.nextWord();order.push_back('C');
   }),"after deadline did not trigger");
  check(order==std::vector<char>({'P','R','C','R'}),"host RC_Pain service must run before the deadline RNG draw");
  check(dueRandom.drawCount==2,"one host-service draw must precede exactly one deadline draw");
  Random expectedRandom(0x12345678u);expectedRandom.nextWord();
  const Extended expectedDelay=(Extended::from(2500.0f)-Extended::from(1000.0f))*
                               Extended::from(expectedRandom.unit())+
                               Extended::from(1000.0f);
  check(deadline==uint32_t((Extended::integer(5001)+expectedDelay).truncate()),
        "deadline must add clock and truncate to low dword");

  Random wrapRandom(0x87654321u);
  deadline=0;
  check(updatePlayerPainDeadline(deadline,0xffffffffu,wrapRandom,[](Random&){}),"high-bit unsigned clock should pass zero deadline");
  check(deadline<uint32_t(0xffffffffu)&&wrapRandom.drawCount==1,"deadline store must retain low 32 bits");

  std::cout<<"Player pain deadline source-derived control flow: failures="<<failures
           <<" (native check only; original 00451640 P-code fixture missing)\n";
  return failures?1:0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 2;}
}
