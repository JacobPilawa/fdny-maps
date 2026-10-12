#include <json-c/json.h>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include "../src/bmodel_use.h"

using J=json_object*;
J field(J j,const char* key){J value=nullptr;json_object_object_get_ex(j,key,&value);return value;}
std::string str(J value){return value?json_object_get_string(value):"";}
int integer(J value){return value?json_object_get_int(value):0;}
bool boolean(J value){return value&&json_object_get_boolean(value);}

int main(int argc,char** argv){
 if(argc!=2){std::cerr<<"usage: bmodel_use_pcode_check fixture.json\n";return 2;}
 J fixture=json_object_from_file(argv[1]);if(!fixture)throw std::runtime_error("Cannot load BModel use P-code fixture");
 if(str(field(fixture,"exe_sha256"))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("BModel use fixture executable hash changed");
 J samples=field(fixture,"samples");int mismatches=0,compared=0;
 for(int i=0;i<json_object_array_length(samples);i++){
  J row=json_object_array_get_idx(samples,i);bool hosePresent=boolean(field(row,"hose_slot_present"));
  bool heldNonNull=boolean(field(row,"held_pointer_nonnull"));int state=integer(field(row,"bmodel_state"));
  auto actual=BModelUse::candidate(1,hosePresent,integer(field(row,"hose_mode")),
   uint8_t(integer(field(row,"player_byte_12c"))),!heldNonNull,state);
  int expected=integer(field(row,"action"));compared++;
  if(actual.action!=expected){mismatches++;std::cout<<str(field(row,"name"))<<" actual="<<actual.action<<" expected="<<expected<<"\n";}
  if(!actual.hasQueryInterface){mismatches++;std::cout<<str(field(row,"name"))<<" lost nonzero-mode query interface\n";}
 }
 J transitions=field(fixture,"transitions");
 for(int i=0;i<json_object_array_length(transitions);i++){
  J row=json_object_array_get_idx(transitions,i);int calls=0,firstArgument=0;bool secondIsNull=false,receiverMatches=false;
  int heldAtCall=0,bmodelAtCall=0,playerAtCall=0;int bmodelObject=0,actionSubobject=0;
  BModelUse::LiftTransition state{nullptr,0,3};
  bool transitioned=BModelUse::enterLiftMode(1,&bmodelObject,&actionSubobject,state,
   [&](void* receiver,std::uintptr_t first,const void* second){calls++;receiverMatches=receiver==&actionSubobject;firstArgument=int(first);secondIsNull=second==nullptr;heldAtCall=state.playerHeldObject==&bmodelObject;bmodelAtCall=state.bmodelObjectState;playerAtCall=state.playerState;});
  auto report=[&](const char* label,bool matched){if(!matched){mismatches++;std::cout<<str(field(row,"name"))<<" transition mismatch: "<<label<<"\n";}};
  report("transition returned",transitioned);
  report("held pointer",(state.playerHeldObject==&bmodelObject)==boolean(field(row,"player_held_object_is_bmodel")));
  report("BModel state",state.bmodelObjectState==integer(field(row,"bmodel_state")));
  report("Player state",state.playerState==integer(field(row,"player_state")));
  report("service called",(calls==1)==boolean(field(row,"helper_called")));
  report("service receiver",receiverMatches&&integer(field(row,"helper_receiver_offset"))==0x5f4);
  report("service arguments",firstArgument==integer(field(row,"helper_argument_1"))&&secondIsNull==(integer(field(row,"helper_argument_2"))==0));
  report("state before service",heldAtCall==boolean(field(row,"held_object_at_helper_is_bmodel"))&&bmodelAtCall==integer(field(row,"bmodel_state_at_helper"))&&playerAtCall==integer(field(row,"player_state_at_helper")));
 }
 std::cout<<"Original BModel use P-code: action-gate cases="<<compared<<", transition cases="<<json_object_array_length(transitions)<<", mismatches="<<mismatches<<"\n";
 return mismatches?1:0;
}
