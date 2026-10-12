#include "../src/player_ash_pool.h"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <json-c/json.h>
#include <stdexcept>
#include <string>

static json_object* field(json_object* object,const char* name){
    json_object* value=nullptr;
    if(!json_object_object_get_ex(object,name,&value))
        throw std::runtime_error(std::string("Missing ash spawn P-code field: ")+name);
    return value;
}
static uint64_t integer(json_object* value){return json_object_get_uint64(value);}
static uint32_t bits(float value){uint32_t word;std::memcpy(&word,&value,sizeof(word));return word;}
static float value(uint32_t word){float result;std::memcpy(&result,&word,sizeof(result));return result;}

int main(int argc,char** argv){
    try{
        if(argc!=2)throw std::runtime_error("Usage: player_ash_spawn_pcode-check ash-spawn-pcode-oracle.json");
        json_object* fixture=json_object_from_file(argv[1]);
        if(!fixture)throw std::runtime_error("Cannot read ash spawn P-code fixture");
        const char* expectedHash="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e";
        if(std::string(json_object_get_string(field(fixture,"exe_sha256")))!=expectedHash)
            throw std::runtime_error("Ash spawn P-code fixture executable hash mismatch");
        json_object* samples=field(fixture,"samples");int failures=0;
        const int sampleCount=int(json_object_array_length(samples));
        for(int i=0;i<sampleCount;i++){
            json_object* sample=json_object_array_get_idx(samples,i);
            PlayerAshFrame frame;
            for(int j=0;j<3;j++){
                frame.position[size_t(j)]=value(uint32_t(integer(json_object_array_get_idx(field(sample,"position_before"),j))));
                frame.direction[size_t(j)]=value(uint32_t(integer(json_object_array_get_idx(field(sample,"direction_before"),j))));
            }
            int frameReads=0;PlayerAshPoolServices services;
            services.getPlayerFrame=[&](Random&){frameReads++;return frame;};
            Random rng(uint32_t(integer(field(sample,"seed"))));
            const float angle=float(rng.range(0.0f,6.2831855f));
            const float distance=float(rng.range(10.0f,400.0f));
            const auto position=PlayerAshPool::spawnPosition(angle,distance,rng,services);
            PlayerAshParticle particle;PlayerAshPool::activate(particle,position,uint32_t(integer(field(sample,"now"))),rng);
            bool mismatch=frameReads!=4||frameReads!=integer(field(sample,"player_getter_calls"))||rng.drawCount!=integer(field(sample,"range_calls"))||
                !json_object_get_boolean(field(sample,"active_after"))||!particle.active||
                particle.activationScale!=value(uint32_t(integer(field(sample,"activation_scale_after"))))||
                particle.opacityScale!=value(uint32_t(integer(field(sample,"opacity_after"))))||
                particle.deadline!=uint32_t(integer(field(sample,"deadline_after")));
            for(int j=0;j<3;j++)
                mismatch=mismatch||bits(position[size_t(j)])!=uint32_t(integer(json_object_array_get_idx(field(sample,"spawn_position_after"),j)))||
                    bits(particle.vector7c[size_t(j)])!=uint32_t(integer(json_object_array_get_idx(field(sample,"vector7c_after"),j)));
            if(mismatch){
                failures++;
                std::cerr<<"Ash spawn P-code mismatch sample="<<json_object_get_string(field(sample,"name"))
                         <<" getter_calls="<<frameReads<<" rng_draws="<<rng.drawCount<<" position_bits="
                         <<bits(position[0])<<","<<bits(position[1])<<","<<bits(position[2])<<"\n";
            }
        }
        json_object_put(fixture);
        std::cout<<"Ash spawn: "<<sampleCount<<" original P-code cases; mismatches="<<failures<<"\n";
        return failures?1:0;
    }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 2;}
}
