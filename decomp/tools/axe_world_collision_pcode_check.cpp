#include <json-c/json.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include "../src/axe_world_collision.h"
using J=json_object*;
static J field(J o,const char*k){J v=nullptr;if(!json_object_object_get_ex(o,k,&v))throw std::runtime_error(std::string("Missing field: ")+k);return v;}
static uint32_t u32(J v){return static_cast<uint32_t>(json_object_get_uint64(v));}
static std::string str(J v){return json_object_get_string(v);}
static bool boolean(J v){return json_object_get_boolean(v)!=0;}
static AxeWorldCollision::Vec3 vec(J v){if(json_object_array_length(v)!=3)throw std::runtime_error("Expected vector3");return {float(json_object_get_double(json_object_array_get_idx(v,0))),float(json_object_get_double(json_object_array_get_idx(v,1))),float(json_object_get_double(json_object_array_get_idx(v,2)))};}
static bool same(const AxeWorldCollision::Vec3&a,const AxeWorldCollision::Vec3&b){for(int i=0;i<3;i++)if(std::memcmp(&a[i],&b[i],4))return false;return true;}
int main(int argc,char**argv){try{
 if(argc!=2)throw std::runtime_error("Usage: axe_world_collision_pcode_check fixture.json");J root=json_object_from_file(argv[1]);if(!root)throw std::runtime_error("Cannot read collision wrapper fixture");
 if(str(field(root,"exe_sha256"))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("Executable hash mismatch");
 J rows=field(root,"samples");int errors=0;
 for(int n=0;n<json_object_array_length(rows);n++){
  J row=json_object_array_get_idx(rows,n),ins=field(row,"inputs"),addresses=field(row,"input_addresses"),outs=field(row,"output_addresses");
  AxeWorldCollision::State state;state.receiver=u32(field(row,"receiver"));state.useB4=boolean(field(row,"active"));uint32_t selected=u32(field(row,"selected_world"));if(state.useB4)state.worldAtPlusB4=selected;else state.worldAtPlus8=selected;
  AxeWorldCollision::Inputs in;in.param1Address=u32(json_object_array_get_idx(addresses,0));in.param2Address=u32(json_object_array_get_idx(addresses,1));in.param3Address=u32(json_object_array_get_idx(addresses,2));in.param4Address=u32(json_object_array_get_idx(addresses,3));in.param5Address=u32(json_object_array_get_idx(outs,0));in.param6Address=u32(json_object_array_get_idx(outs,1));in.param1=vec(json_object_array_get_idx(ins,0));in.param2=vec(json_object_array_get_idx(ins,1));in.param3=vec(json_object_array_get_idx(ins,2));in.param4=vec(json_object_array_get_idx(ins,3));
  if(in.param1Address==in.param2Address||in.param2Address==in.param3Address||in.param3Address==in.param4Address||in.param5Address==in.param6Address)throw std::runtime_error("Fixture aliases distinct wrapper pointer arguments");
  J boundary=field(row,"boundary_vectors");AxeWorldCollision::EngineRequest seen{};int calls=0;bool engineHit=boolean(field(row,"hit"));
  auto result=AxeWorldCollision::invoke(state,in,[&](const auto&request,auto&trace){calls++;seen=request;trace.position=vec(field(row,"expected_engine_position"));trace.normal=vec(field(row,"expected_engine_normal"));return engineHit;});
  bool expectedEntered=boolean(field(row,"collision_boundary_entered"));bool ok=calls==(expectedEntered?1:0)&&result.hit==engineHit&&u32(field(row,"selected_world"))==selected;
  if(expectedEntered){
    J args=field(row,"boundary_arguments");ok=ok&&u32(json_object_array_get_idx(args,0))==selected;
    ok=ok&&seen.world==selected&&seen.contentsMask==u32(field(row,"boundary_mask"))&&seen.collisionType==u32(field(row,"boundary_type"));
    if(json_object_array_length(args)<10)throw std::runtime_error("Collision boundary arguments are incomplete");
    ok=ok&&seen.flags0==u32(json_object_array_get_idx(args,7))&&seen.flags1==u32(json_object_array_get_idx(args,8))&&seen.flags2==u32(json_object_array_get_idx(args,9));
    if(json_object_array_length(boundary)==4){ok=ok&&same(seen.mins,vec(json_object_array_get_idx(boundary,0)))&&same(seen.maxs,vec(json_object_array_get_idx(boundary,1)))&&same(seen.start,vec(json_object_array_get_idx(boundary,2)))&&same(seen.end,vec(json_object_array_get_idx(boundary,3)));}
    else ok=false;
  }
  if(result.hit)ok=ok&&same(result.position,vec(field(row,"position")))&&same(result.normal,vec(field(row,"normal")));
  ok=ok&&result.outputsWritten==boolean(field(row,"outputs_written"));
  if(result.outputsWritten)ok=ok&&result.positionOutputAddress==in.param5Address&&result.normalOutputAddress==in.param6Address;
  if(!ok){std::cerr<<str(field(row,"name"))<<" mismatch; calls="<<calls<<" world="<<seen.world<<" hit="<<result.hit<<"\n";errors++;}
 }
 std::cout<<"Axe world collision P-code cases: "<<json_object_array_length(rows)<<", mismatches: "<<errors<<"\n";return errors?1:0;
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}}
