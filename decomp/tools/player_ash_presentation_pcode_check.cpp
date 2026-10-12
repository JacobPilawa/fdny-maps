#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include "json-c/json.h"
#include "../src/player_ash_presentation.h"

static json_object* field(json_object* o,const char* n){json_object* v=nullptr;if(!json_object_object_get_ex(o,n,&v))throw std::runtime_error(std::string("Missing field: ")+n);return v;}
static uint32_t word(json_object* v){return uint32_t(json_object_get_uint64(v));}
static uint64_t wide(json_object* v){return json_object_get_uint64(v);}
static float f32(uint32_t x){float f;std::memcpy(&f,&x,4);return f;}
static uint32_t bits(float f){uint32_t x;std::memcpy(&x,&f,4);return x;}
static std::array<float,3> vec(json_object* a){std::array<float,3> v{};for(int i=0;i<3;i++)v[size_t(i)]=f32(word(json_object_array_get_idx(a,i)));return v;}
static uint64_t doubleBits(double f){uint64_t x;std::memcpy(&x,&f,8);return x;}

int main(int argc,char**argv){
 try{
  if(argc!=2)throw std::runtime_error("Usage: player_ash_presentation_pcode-check ash-presentation-pcode-oracle.json");
  json_object* root=json_object_from_file(argv[1]);if(!root)throw std::runtime_error("Cannot read ash presentation fixture");
  if(std::string(json_object_get_string(field(root,"exe_sha256")))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("Ash presentation fixture executable hash mismatch");
  auto* frameJson=field(root,"frames");if(json_object_array_length(frameJson)!=5)throw std::runtime_error("Expected five Training ash records");
  auto frames=playerAshTrainingPresentationFrames(0);
  for(size_t i=0;i<frames.size();i++){
   auto* f=json_object_array_get_idx(frameJson,int(i));
   bool sourceMismatch=bits(frames[i].duration)!=word(field(f,"duration"))||
                       bits(frames[i].byteChannel)!=word(field(f,"byte_channel"))||
                       bits(frames[i].alpha)!=word(field(f,"alpha"));
   for(int j=0;j<3;j++)sourceMismatch|=bits(frames[i].color[size_t(j)])!=word(json_object_array_get_idx(field(f,"color"),j));
   if(sourceMismatch)throw std::runtime_error("Native Training ash descriptor words differ from original fixture");
  }
  auto secondContainer=playerAshTrainingPresentationFrames(1);
  for(size_t i=0;i<frames.size();i++){
   if(bits(frames[i].duration)!=bits(secondContainer[i].duration)||bits(frames[i].byteChannel)!=bits(secondContainer[i].byteChannel)||bits(frames[i].alpha)!=bits(secondContainer[i].alpha)||frames[i].color!=secondContainer[i].color)
    throw std::runtime_error("Training ash descriptor containers 0/1 differ in presentation words");
  }
  auto* samples=field(root,"samples");int failures=0;auto count=int(json_object_array_length(samples));
  for(int i=0;i<count;i++){
   auto* row=json_object_array_get_idx(samples,i);std::string name=json_object_get_string(field(row,"name"));
   auto localFrames=frames;PlayerAshParticle particle{};AshPresentationInputs p{};
   particle.animationFlags=word(field(row,"flags"));particle.animationIndex=word(field(row,"current"));particle.nextAnimationIndex=word(field(row,"next"));
   particle.animationTimestamp=word(field(row,"timestamp"));particle.activationScale=f32(word(field(row,"rate_bits")));
   particle.byteChannelScale=f32(word(field(row,"byte_scale_bits")));particle.opacityScale=f32(word(field(row,"opacity_bits")));
   p=playerAshPresentationInputs(particle,word(field(row,"now")));
   if(bits(localFrames.at(p.currentIndex).duration)!=word(field(row,"current_duration_bits")))throw std::runtime_error(name+" duration input differs from native Training descriptor");
   localFrames.at(p.currentIndex).alpha=f32(word(field(row,"alpha_current_bits")));
   localFrames.at(p.nextIndex).alpha=f32(word(field(row,"alpha_next_bits")));

   auto rgb=playerAshPresentationColor(p,localFrames);
   auto channels=playerAshPresentationByteChannels(p,localFrames);
   float scaledAlpha=playerAshPresentationScaledAlpha(p,localFrames,particle.opacityScale);
   auto expectedRgb=field(row,"rgb_bits");auto expectedChannels=field(row,"byte_channels");bool bad=false;
   for(int j=0;j<3;j++)bad|=bits(rgb[size_t(j)])!=word(json_object_array_get_idx(expectedRgb,j));
   for(int j=0;j<2;j++)bad|=channels[size_t(j)]!=json_object_get_int(json_object_array_get_idx(expectedChannels,j));
   uint64_t expectedAlpha=wide(field(row,"alpha_double_bits"));
   bad|=doubleBits(playerAshPresentationAlpha(p,localFrames).asDouble())!=expectedAlpha;
   bad|=bits(scaledAlpha)!=word(field(row,"scaled_alpha_bits"));
   if(bad){failures++;std::cerr<<name<<" mismatch rgb="<<std::hex<<bits(rgb[0])<<","<<bits(rgb[1])<<","<<bits(rgb[2])<<" bytes="<<std::dec<<int(channels[0])<<","<<int(channels[1])<<" alpha64="<<std::hex<<doubleBits(playerAshPresentationAlpha(p,localFrames).asDouble())<<" alpha32="<<bits(scaledAlpha)<<"\n";}
  }
  json_object_put(root);std::cout<<"Ash presentation helpers: "<<count<<" original P-code cases; mismatches="<<failures<<"\n";return failures?1:0;
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}
}
