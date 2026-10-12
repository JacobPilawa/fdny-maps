#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <json-c/json.h>
#include "../src/player_ash_transform.h"

static json_object* field(json_object* object,const char* name){
    json_object* value=nullptr;
    if(!json_object_object_get_ex(object,name,&value))
        throw std::runtime_error(std::string("Missing ash transform P-code field: ")+name);
    return value;
}
static uint64_t integer(json_object* value){return json_object_get_uint64(value);}
static uint32_t bits(float value){uint32_t word;std::memcpy(&word,&value,sizeof(word));return word;}
static float value(uint32_t word){float result;std::memcpy(&result,&word,sizeof(result));return result;}

int main(int argc,char** argv){
    try{
        if(argc!=2)throw std::runtime_error("Usage: player_ash_transform_pcode-check ash-transform-chain-pcode-oracle.json");
        json_object* fixture=json_object_from_file(argv[1]);
        if(!fixture)throw std::runtime_error("Cannot read ash transform P-code fixture");
        const char* expectedHash="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e";
        if(std::string(json_object_get_string(field(fixture,"exe_sha256")))!=expectedHash)
            throw std::runtime_error("Ash transform P-code fixture executable hash mismatch");
        json_object* samples=field(fixture,"samples");
        int failures=0;
        const int sampleCount=int(json_object_array_length(samples));
        for(int i=0;i<sampleCount;i++){
            json_object* sample=json_object_array_get_idx(samples,i);
            std::vector<PlayerAshTransformNode> nodes;
            json_object* inputNodes=field(sample,"nodes");
            for(size_t n=0;n<json_object_array_length(inputNodes);n++){
                json_object* input=json_object_array_get_idx(inputNodes,n);
                PlayerAshTransformNode node;
                node.parent=int(json_object_get_int(field(input,"parent")));
                for(int j=0;j<9;j++)
                    node.value.axis[size_t(j/3)][size_t(j%3)]=value(uint32_t(integer(
                        json_object_array_get_idx(field(input,"axes"),j))));
                for(int j=0;j<3;j++){
                    node.value.position[size_t(j)]=value(uint32_t(integer(
                        json_object_array_get_idx(field(input,"origin"),j))));
                    node.value.scale[size_t(j)]=value(uint32_t(integer(
                        json_object_array_get_idx(field(input,"scale"),j))));
                }
                nodes.push_back(node);
            }
            const PlayerAshTransform resolved=resolvePlayerAshTransform(nodes,nodes.size()-1);
            bool mismatch=integer(field(sample,"resolved_parent"))!=0;
            std::vector<std::string> differences;
            for(int j=0;j<9;j++)
                if(bits(resolved.axis[size_t(j/3)][size_t(j%3)])!=uint32_t(integer(
                    json_object_array_get_idx(field(sample,"resolved_axes"),j)))){
                    mismatch=true;differences.push_back("axis"+std::to_string(j));
                }
            for(int j=0;j<3;j++){
                if(bits(resolved.position[size_t(j)])!=uint32_t(integer(
                    json_object_array_get_idx(field(sample,"resolved_origin"),j)))){
                    mismatch=true;differences.push_back("origin"+std::to_string(j));
                }
                if(bits(resolved.scale[size_t(j)])!=uint32_t(integer(
                    json_object_array_get_idx(field(sample,"resolved_scale"),j)))){
                    mismatch=true;differences.push_back("scale"+std::to_string(j));
                }
            }
            if(mismatch){
                failures++;
                std::cerr<<"Ash transform P-code mismatch sample="
                         <<json_object_get_string(field(sample,"name"))
                         <<" origin="<<bits(resolved.position[0])<<","<<bits(resolved.position[1])
                         <<","<<bits(resolved.position[2])<<" fields=";
                for(const std::string& difference:differences)std::cerr<<difference<<",";
                std::cerr<<"\n";
            }
        }
        json_object_put(fixture);
        std::cout<<"Ash transform: "<<sampleCount<<" original P-code cases; mismatches="<<failures<<"\n";
        return failures?1:0;
    }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 2;}
}
