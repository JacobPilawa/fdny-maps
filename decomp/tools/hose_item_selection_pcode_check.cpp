#include <json-c/json.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include "../src/hose_item_selection.h"

using J=json_object*;
static J field(J object,const char* key){J value=nullptr;if(!json_object_object_get_ex(object,key,&value))throw std::runtime_error(std::string("Missing hose item-selection field: ")+key);return value;}
static std::string string(J value){return json_object_get_string(value);}
static int integer(J value){return json_object_get_int(value);}
static bool boolean(J value){return json_object_get_boolean(value)!=0;}
static bool arrayEqual(J expected,const std::vector<std::string>& actual){if(json_object_array_length(expected)!=actual.size())return false;for(size_t i=0;i<actual.size();i++)if(string(json_object_array_get_idx(expected,int(i)))!=actual[i])return false;return true;}

int main(int argc,char** argv){
 try{
  if(argc!=2)throw std::runtime_error("Usage: hose_item_selection_pcode_check fixture.json");
  J fixture=json_object_from_file(argv[1]);if(!fixture)throw std::runtime_error("Cannot read original hose item-selection P-code fixture");
  if(string(field(fixture,"exe_sha256"))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("Hose item-selection fixture executable hash mismatch");
  J samples=field(fixture,"samples");int mismatches=0,comparedEvents=0;
  for(int i=0;i<json_object_array_length(samples);i++){
   J row=json_object_array_get_idx(samples,i);HoseItemSelection::State state;
   state.selected=integer(field(row,"selected_before"));state.hoseMode=integer(field(row,"hose_mode_before"));
   J slots=field(row,"input_slots"),can=field(row,"can_deactivate");
   for(int slot=0;slot<5;slot++){int marker=integer(json_object_array_get_idx(slots,slot));state.slots[size_t(slot)].present=marker>=0;state.slots[size_t(slot)].canDeactivate=boolean(json_object_array_get_idx(can,slot));}
   state.axeGate.primaryChildEnabled=integer(field(row,"axe_primary_enabled"))!=0;
   state.axeGate.primaryChildState=uint32_t(integer(field(row,"axe_primary_state")));
   state.axeGate.secondaryChildState=uint32_t(integer(field(row,"axe_secondary_state")));
   auto result=integer(field(row,"action"))==1002?HoseItemSelection::detach(state):HoseItemSelection::attach(state);
   const std::string name=string(field(row,"name"));
   auto report=[&](const std::string& label,bool matched){if(!matched){mismatches++;if(mismatches<20)std::cerr<<name<<" mismatch: "<<label<<"\n";}};
   report("selected index",state.selected==integer(field(row,"selected_after")));
   report("hose presence",result.hosePresent==boolean(field(row,"hose_present")));
   if(result.hosePresent)report("hose mode",state.hoseMode==integer(field(row,"hose_mode_after")));
   report("stopped before frame transform",result.reachedFrameTransform==boolean(field(row,"stopped_at_frame_transform")));
   report("returned from owner",result.returned==boolean(field(row,"returned")));
   report("saved valve pointer unchanged",!boolean(field(row,"hose_valve_pointer_is_root")));
   J outputSlots=field(row,"slots");for(int slot=0;slot<5;slot++)report("inventory slot "+std::to_string(slot),integer(json_object_array_get_idx(outputSlots,slot))==integer(json_object_array_get_idx(slots,slot)));
   J itemEvents=field(row,"item_events"),uiEvents=field(row,"ui_events"),ordered=field(row,"ordered_events");
   std::vector<std::string> actualItems,actualUi;for(const auto& event:result.events){if(event.rfind("can:",0)==0||event.rfind("active:",0)==0)actualItems.push_back(event);else actualUi.push_back(event);}
   report("item callback events",arrayEqual(itemEvents,actualItems));report("UI callback events",arrayEqual(uiEvents,actualUi));
   if(!arrayEqual(ordered,result.events)){mismatches++;if(mismatches<20){std::cerr<<name<<" ordered events actual=";for(const auto& event:result.events)std::cerr<<" ["<<event<<"]";std::cerr<<" expected=";for(int k=0;k<json_object_array_length(ordered);k++)std::cerr<<" ["<<string(json_object_array_get_idx(ordered,k))<<"]";std::cerr<<"\n";}}
   comparedEvents+=int(result.events.size());
  }
  J helperSamples=field(fixture,"helper_samples");
  for(int i=0;i<json_object_array_length(helperSamples);i++){
   J row=json_object_array_get_idx(helperSamples,i);HoseItemSelection::State state;state.selected=integer(field(row,"selected_before"));HoseItemSelection::Result result;HoseItemSelection::reselectFirst(state,result);
   if(state.selected!=integer(field(row,"selected_after"))||!result.events.empty()){mismatches++;std::cerr<<string(field(row,"name"))<<" helper mismatch\n";}
  }
  std::cout<<"Hose item selection 0044C840/0044C2A0/0044C360: owner transactions="<<json_object_array_length(samples)<<", helper cases="<<json_object_array_length(helperSamples)<<", ordered boundary events="<<comparedEvents<<", mismatches="<<mismatches<<" (attachment stops before 004977B0)\n";
  json_object_put(fixture);return mismatches?1:0;
 }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 2;}
}
