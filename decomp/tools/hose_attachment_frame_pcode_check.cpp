#include <json-c/json.h>
#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../src/hose_attachment_frame.h"

using J=json_object*;

static J field(J object,const char* key){J value=nullptr;if(!json_object_object_get_ex(object,key,&value))throw std::runtime_error(std::string("Missing hose attachment field: ")+key);return value;}
static std::string string(J value){return json_object_get_string(value);}
static int integer(J value){return json_object_get_int(value);}
static uint32_t word(J value){return uint32_t(json_object_get_int64(value));}
static float fromBits(uint32_t bits){float value;std::memcpy(&value,&bits,sizeof(value));return value;}
static uint32_t toBits(float value){uint32_t bits;std::memcpy(&bits,&value,sizeof(bits));return bits;}
static std::vector<uint32_t> words(J array){std::vector<uint32_t> result;for(int i=0;i<json_object_array_length(array);++i)result.push_back(word(json_object_array_get_idx(array,i)));return result;}
static std::vector<std::string> strings(J array){std::vector<std::string> result;for(int i=0;i<json_object_array_length(array);++i)result.push_back(string(json_object_array_get_idx(array,i)));return result;}
static std::vector<uint32_t> bits(const std::array<float,3>& values){return {toBits(values[0]),toBits(values[1]),toBits(values[2])};}
static std::string show(const std::vector<uint32_t>& values){std::string result="[";for(size_t i=0;i<values.size();++i){if(i)result+=",";result+=std::to_string(values[i]);}return result+"]";}

int main(int argc,char** argv){
 try{
  if(argc!=2)throw std::runtime_error("Usage: hose_attachment_frame_pcode_check fixture.json");
  J fixture=json_object_from_file(argv[1]);if(!fixture)throw std::runtime_error("Cannot read original Hose attachment P-code fixture");
  if(string(field(fixture,"exe_sha256"))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("Hose attachment fixture executable hash mismatch");
  J samples=field(fixture,"samples");int mismatches=0;size_t checkedWords=0;
  for(int i=0;i<json_object_array_length(samples);++i){
   J row=json_object_array_get_idx(samples,i);HoseAttachmentFrame::Input input;
   input.valveRoot=integer(field(row,"valve_root_id"));input.valveFrameRoot=size_t(integer(field(row,"valve_frame_root")));
   input.firstStreamPoseNode=integer(field(row,"first_stream_pose_node_id"));
   auto getter=words(field(row,"valve_getter_position_bits"));if(getter.size()!=3)throw std::runtime_error("Expected three valve getter position words");
   for(size_t axis=0;axis<3;++axis)input.valveGetterPosition[axis]=fromBits(getter[axis]);
   J nodes=field(row,"valve_frame_nodes");
   for(int n=0;n<json_object_array_length(nodes);++n){
    J entry=json_object_array_get_idx(nodes,n);PlayerAshTransformNode node;
    auto axes=words(field(entry,"axes_bits")),position=words(field(entry,"position_bits")),scale=words(field(entry,"scale_bits"));
    if(axes.size()!=9||position.size()!=3||scale.size()!=3)throw std::runtime_error("Bad transform node dimensions");
    for(size_t a=0;a<3;++a){for(size_t b=0;b<3;++b)node.value.axis[a][b]=fromBits(axes[a*3+b]);node.value.position[a]=fromBits(position[a]);node.value.scale[a]=fromBits(scale[a]);}
    node.parent=integer(field(entry,"parent"));input.valveFrameNodes.push_back(node);
   }
   HoseAttachmentFrame::State state;state.hoseMode=integer(field(row,"hose_mode_before"));
   auto result=HoseAttachmentFrame::commit(state,input);
   auto report=[&](const char* label,const std::vector<uint32_t>& actual,J expected){auto want=words(expected);checkedWords+=want.size();if(actual!=want){++mismatches;if(mismatches<12)std::cerr<<string(field(row,"name"))<<" mismatch "<<label<<" actual="<<show(actual)<<" expected="<<show(want)<<"\n";}};
   report("Hose +0xC70 getter position",bits(state.hoseFramePosition),field(row,"hose_frame_position_bits"));
   report("Hose +0xC7C negated resolved valve axis 0",bits(state.hoseValveOffset),field(row,"hose_valve_offset_bits"));
   report("first pose node position",bits(state.streamPosePosition),field(row,"stream_pose_node_position_bits"));
   if(state.hoseMode!=integer(field(row,"hose_mode_after"))){++mismatches;if(mismatches<12)std::cerr<<string(field(row,"name"))<<" mismatch Hose mode\n";}
   if(state.valveRoot!=integer(field(row,"hose_valve_root_id"))){++mismatches;if(mismatches<12)std::cerr<<string(field(row,"name"))<<" mismatch saved valve root\n";}
   if(state.firstStreamPoseNode!=integer(field(row,"hose_current_stream_pose_node_id"))){++mismatches;if(mismatches<12)std::cerr<<string(field(row,"name"))<<" mismatch current pose node\n";}
   const auto expectedEvents=strings(field(row,"ordered_boundary_events"));
   const std::vector<std::string> actualEvents(result.boundaryEvents.begin(),result.boundaryEvents.end());
   if(actualEvents!=expectedEvents){++mismatches;if(mismatches<12)std::cerr<<string(field(row,"name"))<<" mismatch ordered boundary events\n";}
  }
  std::cout<<"Hose attachment frame 0044C840/004977B0: owner commits="<<json_object_array_length(samples)<<", compared words="<<checkedWords<<", mismatches="<<mismatches<<" (item setup, real valve frame source and live scheduler are inputs)\n";
  json_object_put(fixture);return mismatches?1:0;
 }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 2;}
}
