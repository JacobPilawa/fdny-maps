#include <json-c/json.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../src/hose_item_selection.h"

using J=json_object*;
static J field(J o,const char*k){J v=nullptr;if(!json_object_object_get_ex(o,k,&v))throw std::runtime_error(std::string("Missing Axe activation field: ")+k);return v;}
static std::string str(J v){return json_object_get_string(v);}
static int integer(J v){return json_object_get_int(v);}
static bool sameStrings(J expected,const std::vector<std::string>& actual){if(json_object_array_length(expected)!=actual.size())return false;for(size_t i=0;i<actual.size();i++)if(str(json_object_array_get_idx(expected,int(i)))!=actual[i])return false;return true;}

int main(int argc,char**argv){try{
 if(argc!=2)throw std::runtime_error("Usage: hose_axe_activation_pcode_check fixture.json");
 J fixture=json_object_from_file(argv[1]);if(!fixture)throw std::runtime_error("Cannot read original Axe activation P-code fixture");
 if(str(field(fixture,"exe_sha256"))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("Axe activation fixture executable hash mismatch");
 J samples=field(fixture,"samples");int mismatches=0,comparedServices=0;
 for(int i=0;i<json_object_array_length(samples);i++){
  J row=json_object_array_get_idx(samples,i);HoseItemSelection::State state;bool active=integer(field(row,"active"))!=0;state.axeGate.primaryChildEnabled=integer(field(row,"primary_enabled"))!=0;state.axeActivation.player345c=0x7f;
  HoseItemSelection::Result result;HoseItemSelection::axeItemActive(state,active,result);std::vector<std::string> actualServices;
  for(auto&e:result.axeCallbackEvents)if(e.rfind("animation:",0)==0||e.rfind("reparent:",0)==0)actualServices.push_back(e);
  std::string name=str(field(row,"name"));auto report=[&](const char*label,bool ok){if(!ok){mismatches++;if(mismatches<20)std::cerr<<name<<" mismatch: "<<label<<"\n";}};
  report("Axe active byte",state.axeActivation.rootActive==(integer(field(row,"axe_active"))!=0));
  report("primary child active byte",state.axeActivation.primaryChildActive==(integer(field(row,"primary_active"))!=0));
  report("secondary child active byte",state.axeActivation.secondaryChildActive==(integer(field(row,"secondary_active"))!=0));
  report("Player+0x345C",state.axeActivation.player345c==integer(field(row,"player_345c")));
  report("ordered animation/reparent calls",sameStrings(field(row,"events"),actualServices));
  report("ordered active-byte/Player-byte snapshots at animation calls",sameStrings(field(row,"animation_snapshots"),result.axeAnimationSnapshots));
  // Child callback semantics place Player+0x345C clear after its animation on deactivation.
  if(!active){auto anim=std::find(result.axeCallbackEvents.begin(),result.axeCallbackEvents.end(),"animation:1696:0:0");auto clear=std::find(result.axeCallbackEvents.rbegin(),result.axeCallbackEvents.rend(),"player-byte:13404:0:0");report("secondary child clears after animation",anim!=result.axeCallbackEvents.end()&&clear!=result.axeCallbackEvents.rend()&&anim<std::prev(clear.base()));}
  comparedServices+=int(actualServices.size());
 }
 std::cout<<"Training Axe +0x3C activation/deactivation: cases="<<json_object_array_length(samples)<<", ordered engine calls="<<comparedServices<<", mismatches="<<mismatches<<" (00497DE0 animation and 00483A20 reparent remain service boundaries)\n";
 json_object_put(fixture);return mismatches?1:0;
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}}
