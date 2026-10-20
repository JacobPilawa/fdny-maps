#include <json-c/json.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
using J=json_object*;
J field(J j,const char* k){J v=nullptr;if(j)json_object_object_get_ex(j,k,&v);return v;}
J at(J j,int i){return json_object_array_get_idx(j,i);}
int count(J j){return j?json_object_array_length(j):0;}
float num(J j){return j?json_object_get_double(j):0;}
int integer(J j){return j?json_object_get_int(j):0;}
#include "core.h"
void check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
int main(int argc,char** argv){
 J data=json_object_from_file(argv[1]);check(data,"read collision");Collision c(data);Hit h;
 check(!c.solid({0,32,400},{-30,-30,-30},{30,85,30}),"spawn must be clear");
 check(c.trace({0,100,400},{0,-300,0},{-30,-30,-30},{30,85,30},h),"floor sweep");
 check(h.normal.y>.99&&std::abs(h.point.y-30)<1,"floor height and normal");
 h={};check(c.trace({0,32,400},{1000,0,0},{-30,-30,-30},{30,85,30},h),"wall sweep");
 check(h.normal.x<-.99&&h.point.x<740,"wall blocks player");
 h={};check(c.tracePoint({0,100,400},{0,-100,400},1,2,h),"point floor trace used by fire and survivor placement");
 check(h.normal.y>.99&&std::abs(h.point.y)<1,"point floor height");
 if(argc>2){
  h={};check(c.trace({0,200,0},{0,-250,0},{-30,-30,-30},{30,85,30},h),"ramp sweep");
  check(h.normal.y>.8&&std::abs(h.normal.z)>.2&&h.point.y>85,"sloped collision surface");
  Player p;p.p={0,32,-260};for(int i=0;i<150;i++)p.step(1.f/120,-1,0,0,false,false,false,c);
  check(p.p.z>0&&p.p.y>90,"walk uphill on ramp");
 }else{Player p;p.p={0,32,400};for(int i=0;i<600;i++)p.step(1.f/120,0,1,0,false,false,false,c);check(p.p.x<740&&p.p.x>700&&p.ground,"walking stops at wall and stays grounded");}
 json_object_put(data);std::cout<<"PASS native floor, wall, point traces, movement"<<(argc>2?", ramp ascent":"")<<"\n";
}
