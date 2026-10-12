#include <json-c/json.h>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../src/bmodel_candidate_iterator.h"

using J=json_object*;
struct FixtureMemory final:BModelCandidateIterator::Memory {
    std::vector<std::pair<uint32_t,uint32_t>> words;
    uint32_t read32(uint32_t p)const override{for(auto it=words.rbegin();it!=words.rend();++it)if(it->first==p)return it->second;return 0;}
    void put(uint32_t p,uint32_t v){words.emplace_back(p,v);}
};
static J field(J o,const char*k){J v=nullptr;if(!json_object_object_get_ex(o,k,&v))throw std::runtime_error(std::string("Missing field: ")+k);return v;}
static uint32_t u32(J v){return static_cast<uint32_t>(json_object_get_uint64(v));}
static std::string str(J v){return json_object_get_string(v);}
int main(int argc,char**argv){try{
 if(argc!=2)throw std::runtime_error("Usage: bmodel_candidate_iterator_pcode_check fixture.json");J f=json_object_from_file(argv[1]);if(!f)throw std::runtime_error("Cannot read original iterator fixture");
 if(str(field(f,"exe_sha256"))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("Iterator fixture executable hash mismatch");
 J samples=field(f,"samples");int mismatches=0;const size_t sampleCount=json_object_array_length(samples);for(size_t i=0;i<sampleCount;i++){J row=json_object_array_get_idx(samples,int(i));FixtureMemory mem;uint32_t head=0;J input=field(row,"controlled_input_objects");std::vector<uint32_t>nodes,objects;const size_t inputCount=json_object_array_length(input);for(size_t j=0;j<inputCount;j++){objects.push_back(u32(json_object_array_get_idx(input,int(j))));nodes.push_back(0x20020010u+uint32_t(j)*8);}for(size_t j=0;j<nodes.size();j++){mem.put(nodes[j],objects[j]);mem.put(nodes[j]+4,j+1<nodes.size()?nodes[j+1]:0);}if(!nodes.empty())head=nodes.front();const uint32_t manager=0x20010000u,list=manager+0x54;mem.put(list+0x10,head);BModelCandidateIterator::Cursor c;BModelCandidateIterator::initialize(c,manager,mem);std::vector<uint32_t>order;BModelCandidateIterator::each(c,mem,[&](uint32_t p){order.push_back(p);});auto report=[&](bool ok,const char*what){if(!ok){++mismatches;std::cerr<<str(field(row,"name"))<<" mismatch: "<<what<<"\n";}};report(c.list==u32(field(row,"list_address")),"original cursor list address");report((nodes.empty()?0:nodes.front())==u32(field(row,"first_node")),"original cursor first-node initialization");J expected=field(row,"controlled_expected_order");const size_t expectedCount=json_object_array_length(expected);report(order.size()==expectedCount,"controlled linked-list traversal length");for(size_t j=0;j<order.size()&&j<expectedCount;j++)report(order[j]==u32(json_object_array_get_idx(expected,int(j))),"controlled linked-list pointer identity/order");}
 std::cout<<"BModel candidate iterator: cases="<<json_object_array_length(samples)<<", mismatches="<<mismatches<<" (original P-code cursor fields checked; traversal checked against controlled/source-derived links)\n";json_object_put(f);return mismatches?1:0;
}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}}
