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
        throw std::runtime_error(std::string("Missing ash geometry P-code field: ")+name);
    return value;
}
static uint64_t integer(json_object* value){return json_object_get_uint64(value);}
static uint32_t bits(float value){uint32_t word;std::memcpy(&word,&value,sizeof(word));return word;}
static float value(uint32_t word){float result;std::memcpy(&result,&word,sizeof(result));return result;}

int main(int argc,char** argv){
    try{
        if(argc!=2)throw std::runtime_error("Usage: player_ash_geometry_pcode-check ash-geometry-pcode-oracle.json");
        json_object* fixture=json_object_from_file(argv[1]);
        if(!fixture)throw std::runtime_error("Cannot read ash geometry P-code fixture");
        const char* expectedHash="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e";
        if(std::string(json_object_get_string(field(fixture,"exe_sha256")))!=expectedHash)
            throw std::runtime_error("Ash geometry P-code fixture executable hash mismatch");
        json_object* samples=field(fixture,"samples");
        int failures=0;
        const int sampleCount=int(json_object_array_length(samples));
        for(int i=0;i<sampleCount;i++){
            json_object* sample=json_object_array_get_idx(samples,i);
            PlayerAshParticle particle;
            particle.geometryDirty=true;
            PlayerAshTransform frame;
            for(int j=0;j<3;j++){
                particle.vectorB4[size_t(j)]=value(uint32_t(integer(json_object_array_get_idx(field(sample,"size_before"),j))));
                frame.position[size_t(j)]=value(uint32_t(integer(json_object_array_get_idx(field(sample,"origin_before"),j))));
                frame.scale[size_t(j)]=value(uint32_t(integer(json_object_array_get_idx(field(sample,"scale_before"),j))));
            }
            for(int j=0;j<9;j++)
                frame.axis[size_t(j/3)][size_t(j%3)]=value(uint32_t(integer(json_object_array_get_idx(field(sample,"axes_before"),j))));

            bool built=PlayerAshPool::buildCachedGeometry(particle,frame);
            bool mismatch=!built||particle.geometryDirty!=bool(json_object_get_boolean(field(sample,"geometry_dirty_after")));
            for(int vertex=0;vertex<4;vertex++)
                for(int component=0;component<3;component++){
                    uint32_t expected=uint32_t(integer(json_object_array_get_idx(
                        json_object_array_get_idx(field(sample,"vertices_after"),vertex),component)));
                    if(bits(particle.geometryPositions[size_t(vertex)][size_t(component)])!=expected)
                        mismatch=true;
                }
            if(PlayerAshPool::buildCachedGeometry(particle,frame))mismatch=true;
            if(mismatch){
                failures++;
                std::cerr<<"Ash geometry P-code mismatch sample="<<json_object_get_string(field(sample,"name"))
                         <<" vertex0="<<bits(particle.geometryPositions[0][0])<<","<<bits(particle.geometryPositions[0][1])
                         <<","<<bits(particle.geometryPositions[0][2])<<" dirty="<<particle.geometryDirty<<"\n";
            }
        }
        json_object_put(fixture);
        std::cout<<"Ash geometry: "<<sampleCount<<" original P-code cases; mismatches="<<failures<<"\n";
        return failures?1:0;
    }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 2;}
}
