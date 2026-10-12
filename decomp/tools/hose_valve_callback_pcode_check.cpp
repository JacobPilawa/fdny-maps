#include <json-c/json.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include "../src/hose_valve_callback.h"

using J=json_object*;
static J field(J object,const char* key){J value=nullptr;if(!json_object_object_get_ex(object,key,&value))throw std::runtime_error(std::string("Missing hose-valve oracle field: ")+key);return value;}
static int integer(J value){return json_object_get_int(value);}
static std::string string(J value){return json_object_get_string(value);}

int main(int argc,char** argv){
 try{
  if(argc!=2)throw std::runtime_error("Usage: hose_valve_callback_pcode_check fixture.json");
  J fixture=json_object_from_file(argv[1]);if(!fixture)throw std::runtime_error("Cannot read original hose-valve callback P-code fixture");
  if(string(field(fixture,"exe_sha256"))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("Hose-valve callback fixture executable hash mismatch");
  J samples=field(fixture,"samples");int mismatches=0;
  for(int i=0;i<json_object_array_length(samples);i++){
   J row=json_object_array_get_idx(samples,i);auto actual=HoseValveCallback::invoke(integer(field(row,"prop_model_state")),integer(field(row,"action")));
   const bool call=integer(field(row,"owner_action_calls"))!=0,forward=integer(field(row,"script_use_calls"))!=0;
   if(actual.callOwnerAction!=call||actual.forwardUseEvent!=forward||
      (call&&(integer(field(row,"owner_receiver_offset"))!=0||integer(field(row,"valve_argument_root_offset"))!=0||integer(field(row,"owner_action"))!=integer(field(row,"action"))))||
      (forward&&integer(field(row,"script_receiver_root_offset"))!=0xd4)){
    ++mismatches;std::cerr<<string(field(row,"name"))<<" owner="<<actual.callOwnerAction<<"/"<<call<<" use="<<actual.forwardUseEvent<<"/"<<forward<<"\n";
   }
  }
  std::cout<<"Valve callback 00444120: "<<json_object_array_length(samples)<<" original P-code cases; attach call/early detach return/event forwarding; mismatches="<<mismatches<<"\n";
  json_object_put(fixture);return mismatches?1:0;
 }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 2;}
}
