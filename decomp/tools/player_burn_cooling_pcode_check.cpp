#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <json-c/json.h>
#include "../src/player_burn_cooling.h"

static json_object* field(json_object* object,const char* name){
 json_object* value=nullptr;
 if(!json_object_object_get_ex(object,name,&value))throw std::runtime_error(std::string("Missing burn-cooling P-code field: ")+name);
 return value;
}
static uint32_t integer32(json_object* value){return uint32_t(json_object_get_uint64(value));}
static float floatBits(json_object* value){uint32_t word=integer32(value);float out;std::memcpy(&out,&word,4);return out;}
static uint32_t bits(float value){uint32_t word;std::memcpy(&word,&value,4);return word;}
template<size_t N> static std::array<uint32_t,N> words(json_object* value){
 if(json_object_array_length(value)!=N)throw std::runtime_error("Bad burn-cooling P-code array length");
 std::array<uint32_t,N> out{};for(size_t i=0;i<N;i++)out[i]=integer32(json_object_array_get_idx(value,int(i)));return out;
}

int main(int argc,char** argv){
 try{
  if(argc!=2)throw std::runtime_error("Usage: player_burn_cooling_pcode-check burn-cooling-pcode-oracle.json");
  json_object* fixture=json_object_from_file(argv[1]);if(!fixture)throw std::runtime_error("Cannot read original burn-cooling P-code fixture");
  const char* expectedHash="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e";
  if(std::string(json_object_get_string(field(fixture,"exe_sha256")))!=expectedHash)throw std::runtime_error("Burn-cooling fixture executable hash mismatch");
  auto* samples=field(fixture,"samples");const int count=int(json_object_array_length(samples));int failures=0;
  for(int i=0;i<count;i++){
   auto* sample=json_object_array_get_idx(samples,i);PlayerBurnCoolingState state;
   state.accumulator=floatBits(field(sample,"accumulator_before_bits"));
   state.statusMarker=uint8_t(json_object_get_int(field(sample,"status_marker_before")));
   auto initialFlags=words<4>(field(sample,"flags_before"));for(int j=0;j<4;j++)state.directionalIndicators[size_t(j)]=uint8_t(initialFlags[size_t(j)]);
   state.copySource=words<3>(field(sample,"source_words_before"));state.copyTarget=words<3>(field(sample,"target_words_before"));
   bool copyCalled=false;uint32_t copyBurnBits=0;
   const auto result=updatePlayerBurnCooling(state,floatBits(field(sample,"dt_bits")),
       json_object_get_int(field(sample,"side_flag_before"))!=0,
       floatBits(field(sample,"side_threshold_before_bits")),[&](const auto& source,auto& target){
        copyCalled=true;copyBurnBits=bits(state.accumulator);target=source;
       });
   auto expectedFlags=words<4>(field(sample,"flags_after"));
   auto expectedSource=words<3>(field(sample,"source_words_after"));
   auto expectedTarget=words<3>(field(sample,"target_words_after"));
   const bool expectedCopy=json_object_get_int(field(sample,"copy_calls"))!=0;
   const bool mismatch=bits(state.accumulator)!=integer32(field(sample,"accumulator_after_bits"))||
       state.directionalIndicators!=std::array<uint8_t,4>{uint8_t(expectedFlags[0]),uint8_t(expectedFlags[1]),uint8_t(expectedFlags[2]),uint8_t(expectedFlags[3])}||
       state.statusMarker!=uint8_t(json_object_get_int(field(sample,"status_marker_after")))||
       state.copySource!=expectedSource||state.copyTarget!=expectedTarget||
       result.cooled!=bool(json_object_get_boolean(field(sample,"cooling_applied")))||
       bits(result.beforeClamp)!=integer32(field(sample,"pre_clamp_bits"))||
       result.clamped!=(json_object_get_int(field(sample,"clamp_calls"))!=0)||
       result.sideCopyCalled!=expectedCopy||copyCalled!=expectedCopy||
       (expectedCopy&&(copyBurnBits!=integer32(field(sample,"copy_accumulator_bits"))||
         integer32(field(sample,"copy_receiver_offset"))!=0x3400||integer32(field(sample,"copy_source_offset"))!=0x4c));
   if(mismatch){failures++;std::cerr<<"Burn-cooling P-code mismatch sample="<<json_object_get_string(field(sample,"name"))
      <<" accumulator_bits="<<bits(state.accumulator)<<" copy="<<copyCalled<<" copy_accumulator="<<copyBurnBits<<"\n";}
  }
  json_object_put(fixture);
  std::cout<<"Player 00452570 burn-cooling slice: "<<count<<" original P-code cases; exact helper-copy boundary, x87 threshold and flag/clamp stores; mismatches="<<failures<<"\n";
  return failures?1:0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 2;}
}
