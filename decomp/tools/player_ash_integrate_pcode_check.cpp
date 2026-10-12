#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <json-c/json.h>
#include "../src/player_ash_integrate.h"

static json_object* field(json_object* o,const char* n){json_object* v=nullptr;if(!json_object_object_get_ex(o,n,&v))throw std::runtime_error(std::string("Missing field: ")+n);return v;}
static uint32_t word(json_object* v){return uint32_t(json_object_get_uint64(v));}
static float f32(uint32_t x){float f;std::memcpy(&f,&x,4);return f;}
static uint32_t bits(float f){uint32_t x;std::memcpy(&x,&f,4);return x;}
static std::array<float,3> vec(json_object* a){std::array<float,3> v{};for(int i=0;i<3;i++)v[size_t(i)]=f32(word(json_object_array_get_idx(a,i)));return v;}
static std::array<uint32_t,3> words(json_object* a){std::array<uint32_t,3> v{};for(int i=0;i<3;i++)v[size_t(i)]=word(json_object_array_get_idx(a,i));return v;}
int main(int argc,char**argv){
 try{
  if(argc!=2)throw std::runtime_error("Usage: player_ash_integrate_pcode-check ash-integrate-pcode-oracle.json");
  json_object* root=json_object_from_file(argv[1]);if(!root)throw std::runtime_error("Cannot read ash integrate fixture");
  if(std::string(json_object_get_string(field(root,"exe_sha256")))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("Ash integrate fixture executable hash mismatch");
  json_object* samples=field(root,"samples");int failures=0;auto count=int(json_object_array_length(samples));
  for(int i=0;i<count;i++){
   auto* row=json_object_array_get_idx(samples,i);PlayerAshParticle particle;
   particle.vector7c=vec(field(row,"position_before"));particle.vector88=vec(field(row,"velocity_before"));particle.vector94=vec(field(row,"acceleration_before"));particle.spawnPosition=vec(field(row,"origin_before"));
   playerAshIntegrateParticle(particle,f32(word(field(row,"dt"))));
   auto ep=words(field(row,"position_after")),eo=words(field(row,"origin_after"));bool bad=false;
   for(int j=0;j<3;j++)bad|=bits(particle.vector7c[size_t(j)])!=ep[size_t(j)]||bits(particle.spawnPosition[size_t(j)])!=eo[size_t(j)];
   if(bad){failures++;std::cerr<<json_object_get_string(field(row,"name"))<<" mismatch position="<<std::hex<<bits(particle.vector7c[0])<<","<<bits(particle.vector7c[1])<<","<<bits(particle.vector7c[2])<<" origin="<<bits(particle.spawnPosition[0])<<","<<bits(particle.spawnPosition[1])<<","<<bits(particle.spawnPosition[2])<<std::dec<<"\n";}
  }
  json_object_put(root);std::cout<<"Ash 0047F2A0: "<<count<<" original P-code cases; mismatches="<<failures<<"\n";return failures?1:0;
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}
}
