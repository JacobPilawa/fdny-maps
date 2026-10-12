#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <json-c/json.h>
#include "../src/player_ash_pool.h"

static json_object* field(json_object* object,const char* name){
    json_object* value=nullptr;
    if(!json_object_object_get_ex(object,name,&value))
        throw std::runtime_error(std::string("Missing ash activation P-code field: ")+name);
    return value;
}
static uint64_t integer(json_object* value){return json_object_get_uint64(value);}
static uint32_t bits(float value){uint32_t word;std::memcpy(&word,&value,sizeof(word));return word;}
static float value(uint32_t word){float result;std::memcpy(&result,&word,sizeof(result));return result;}

int main(int argc,char** argv){
    try{
        if(argc!=2)throw std::runtime_error("Usage: player_ash_activation_pcode-check ash-activate-pcode-oracle.json");
        json_object* fixture=json_object_from_file(argv[1]);
        if(!fixture)throw std::runtime_error("Cannot read ash activation P-code fixture");
        const char* expectedHash="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e";
        if(std::string(json_object_get_string(field(fixture,"exe_sha256")))!=expectedHash)
            throw std::runtime_error("Ash activation P-code fixture executable hash mismatch");
        json_object* samples=field(fixture,"samples");
        int failures=0;
        const int sampleCount=int(json_object_array_length(samples));
        for(int i=0;i<sampleCount;i++){
            json_object* sample=json_object_array_get_idx(samples,i);
            PlayerAshParticle p;
            p.active=false;p.positionInitialized=false;p.vectorsDirty=false;
            p.animationIndex=9;p.nextAnimationIndex=9;p.animationTimestamp=0x87654321u;
            p.deadline=0x12345678u;
            std::array<float,3> position{};
            for(int j=0;j<3;j++)position[size_t(j)]=value(uint32_t(integer(json_object_array_get_idx(field(sample,"position_before"),j))));
            Random rng(uint32_t(integer(field(sample,"seed"))));
            PlayerAshPool::activate(p,position,uint32_t(integer(field(sample,"now"))),rng);
            bool mismatch=
                p.active!=bool(json_object_get_boolean(field(sample,"active_after")))||
                p.positionInitialized!=bool(json_object_get_boolean(field(sample,"position_initialized_after")))||
                p.vectorsDirty!=bool(json_object_get_boolean(field(sample,"dirty_after")))||
                p.animationIndex!=uint32_t(integer(field(sample,"animation_index_after")))||
                p.nextAnimationIndex!=uint32_t(integer(field(sample,"next_animation_index_after")))||
                p.animationTimestamp!=uint32_t(integer(field(sample,"animation_timestamp_after")))||
                bits(p.activationScale)!=uint32_t(integer(field(sample,"activation_scale_after")))||
                bits(p.opacityScale)!=uint32_t(integer(field(sample,"opacity_after")))||
                p.deadline!=uint32_t(integer(field(sample,"deadline_after")))||
                rng.drawCount!=integer(field(sample,"range_calls"));
            for(int j=0;j<3;j++){
                uint32_t expectedPosition=uint32_t(integer(json_object_array_get_idx(field(sample,"spawn_position_after"),j)));
                uint32_t expectedVector=uint32_t(integer(json_object_array_get_idx(field(sample,"vector7c_after"),j)));
                if(bits(p.spawnPosition[size_t(j)])!=expectedPosition)mismatch=true;
                if(bits(p.vector7c[size_t(j)])!=expectedVector)mismatch=true;
            }
            if(mismatch){
                failures++;
                std::cerr<<"Ash activation mismatch sample="<<json_object_get_string(field(sample,"name"))
                         <<" draws="<<rng.drawCount
                         <<" animation="<<p.animationIndex<<","<<p.nextAnimationIndex
                         <<" scale="<<bits(p.activationScale)<<" opacity="<<bits(p.opacityScale)
                         <<" velocityY="<<bits(p.vector7c[1])<<"\n";
            }
        }
        json_object_put(fixture);
        std::cout<<"Ash activation: "<<sampleCount<<" original P-code cases; mismatches="<<failures<<"\n";
        return failures?1:0;
    }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 2;}
}
