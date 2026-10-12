#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include "json-c/json.h"
#include "../src/player_ash_polygon.h"

static json_object* field(json_object* o,const char* n){json_object* v=nullptr;if(!json_object_object_get_ex(o,n,&v))throw std::runtime_error(std::string("Missing field: ")+n);return v;}
static uint32_t word(json_object* v){return uint32_t(json_object_get_uint64(v));}
static float f32(uint32_t x){float f;std::memcpy(&f,&x,sizeof(f));return f;}

int main(int argc,char**argv){
 try{
  if(argc!=2)throw std::runtime_error("Usage: player_ash_poly_pcode-check ash-poly-pcode-oracle.json");
  json_object* root=json_object_from_file(argv[1]);if(!root)throw std::runtime_error("Cannot read ash polygon fixture");
  if(std::string(json_object_get_string(field(root,"exe_sha256")))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("Ash polygon fixture executable hash mismatch");
  const std::array<uint32_t,8> uvExpected{0,0,0x3f7d70a4,0,0x3f7d70a4,0x3f7d70a4,0,0x3f7d70a4};
  auto* uv=field(root,"uv_bits");if(json_object_array_length(uv)!=uvExpected.size())throw std::runtime_error("Expected eight constructor UV words");
  for(size_t i=0;i<uvExpected.size();i++)if(word(json_object_array_get_idx(uv,int(i)))!=uvExpected[i])throw std::runtime_error("Fixture UV words differ from 0047F850 constructor evidence");
  auto frames=playerAshTrainingPresentationFrames(0);auto other=playerAshTrainingPresentationFrames(1);
  for(size_t i=0;i<frames.size();i++)if(frames[i].duration!=other[i].duration||frames[i].byteChannel!=other[i].byteChannel||frames[i].alpha!=other[i].alpha||frames[i].color!=other[i].color)throw std::runtime_error("Training descriptor containers disagree");
  auto* samples=field(root,"samples");int count=int(json_object_array_length(samples)),failures=0;
  for(int i=0;i<count;i++){
   auto* row=json_object_array_get_idx(samples,i);std::string name=json_object_get_string(field(row,"name"));
   if(json_object_get_int(field(row,"submit_count"))!=1)throw std::runtime_error(name+" did not intercept exactly one polygon submission");
   if(std::string(json_object_get_string(field(row,"world")))!="21002300"||std::string(json_object_get_string(field(row,"resource")))!="21002400")throw std::runtime_error(name+" did not retain the explicit fake world/resource selection");
   auto* submitArgs=field(row,"submit_args");if(json_object_array_length(submitArgs)!=7)throw std::runtime_error(name+" expected seven captured cdecl arguments");
   const int state=json_object_get_int(field(row,"state"));
   const int resourceId=json_object_get_int(field(row,"resource_id"));
   const int descriptorId=json_object_get_int(field(row,"descriptor_id"));
   if((resourceId!=200&&resourceId!=201)||descriptorId!=resourceId)throw std::runtime_error(name+" descriptor/resource IDs do not match the controlled table lookup");
   const std::array<uint32_t,7> expectedArgs{0x21002300,0x210000d8,uint32_t(state==0?1:4),0x21002400,uint32_t(state==0?2:0),0x21002500,0x3e4ccccd};
   for(size_t arg=0;arg<expectedArgs.size();arg++)if(word(json_object_array_get_idx(submitArgs,int(arg)))!=expectedArgs[arg])throw std::runtime_error(name+" intercepted submission operands differ at arg "+std::to_string(arg));
   PlayerAshParticle particle{};auto* spawn=field(row,"spawn_position_bits");if(json_object_array_length(spawn)!=3)throw std::runtime_error(name+" expected three spawn position words");
   for(int axis=0;axis<3;axis++)particle.spawnPosition[size_t(axis)]=f32(word(json_object_array_get_idx(spawn,axis)));
   auto* pos=field(row,"positions_bits");if(json_object_array_length(pos)!=12)throw std::runtime_error(name+" expected four XYZ inputs");
   for(int v=0;v<4;v++)for(int axis=0;axis<3;axis++)particle.geometryPositions[size_t(v)][size_t(axis)]=f32(word(json_object_array_get_idx(pos,v*3+axis)));
   AshPresentationInputs inputs{};inputs.flags=word(field(row,"flags"));inputs.currentIndex=word(field(row,"current"));inputs.nextIndex=word(field(row,"next"));inputs.timestamp=word(field(row,"timestamp"));inputs.animationRate=1.0f;inputs.byteChannelScale=1.0f;inputs.now=word(field(row,"now"));
   auto actual=playerAshPolygonWords(playerAshTrainingPolygonPayload(particle,uint32_t(state),inputs,frames));
   auto* expected=field(row,"vertices_bits");if(json_object_array_length(expected)!=actual.size())throw std::runtime_error(name+" expected 36 payload words");
   bool bad=false;for(size_t k=0;k<actual.size();k++)bad|=actual[k]!=word(json_object_array_get_idx(expected,int(k)));
   if(bad){failures++;std::cerr<<name<<" payload mismatch\n";for(size_t v=0;v<4;v++){std::cerr<<"  vertex "<<v<<":";for(size_t k=0;k<9;k++)std::cerr<<" "<<std::hex<<actual[v*9+k]<<"/"<<word(json_object_array_get_idx(expected,int(v*9+k)));std::cerr<<std::dec<<"\n";}}
  }
  json_object_put(root);std::cout<<"Ash polygon payload: "<<count<<" original 0047F960 P-code captures; mismatches="<<failures<<"\n";return failures?1:0;
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}
}
