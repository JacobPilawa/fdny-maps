#pragma once
#include <optional>
struct V{float x=0,y=0,z=0;float& operator[](int i){return i==0?x:i==1?y:z;} float operator[](int i)const{return i==0?x:i==1?y:z;}};
V vec(J j){return {num(at(j,0)),num(at(j,1)),num(at(j,2))};}
V operator+(V a,V b){return {a.x+b.x,a.y+b.y,a.z+b.z};} V operator-(V a,V b){return {a.x-b.x,a.y-b.y,a.z-b.z};} V operator*(V a,float f){return {a.x*f,a.y*f,a.z*f};}
float dot(V a,V b){return float((double)a.x*b.x+(double)a.y*b.y+(double)a.z*b.z);} float length(V a){return std::sqrt(dot(a,a));}
#include "motion.h"
struct Hit{V point,normal;float distance=99999;int model=-1;};
struct Collision{
 J data;std::set<int> disabled;std::map<int,float> angles;std::map<int,Pose> poses;
 V rotate(V p,V origin,float a){if(a==0)return p;float c=std::cos(a),sn=std::sin(a);V v=p-origin;return origin+V{c*v.x+sn*v.z,v.y,-sn*v.x+c*v.z};}
 V transform(V p,V origin,int model,bool inverse){auto it=poses.find(model);if(it==poses.end())return rotate(p,origin,inverse?-angles[model]:angles[model]);Pose pose=it->second;if(inverse){p=p-pose.translation;pose.rotation.x=-pose.rotation.x;pose.rotation.y=-pose.rotation.y;pose.rotation.z=-pose.rotation.z;}V result=rotateQ(p-origin,pose.rotation)+origin;return inverse?result:result+pose.translation;}
 V transformNormal(V n,int model){auto it=poses.find(model);return it==poses.end()?rotate(n,{},angles[model]):rotateQ(n,it->second.rotation);}
 explicit Collision(J d):data(d){}
 void leaves(int index,V mn,V mx,std::vector<int>& out){
 if(index<0){out.push_back(-index-1);return;} auto n=at(field(data,"nodes"),index);auto p=at(field(data,"planes"),integer(field(n,"plane")));V normal=vec(field(p,"normal"));float d=num(field(p,"distance"));
 V near,far;for(int i=0;i<3;i++){near[i]=normal[i]>=0?mn[i]:mx[i];far[i]=normal[i]>=0?mx[i]:mn[i];}
 int type=integer(field(p,"type"));auto planeDot=[&](V v){return type<3?double(v[type]):double(normal.x)*v.x+double(normal.y)*v.y+double(normal.z)*v.z;};
 if(planeDot(far)>=d)leaves(integer(at(field(n,"children"),0)),mn,mx,out);
 if(planeDot(near)<d)leaves(integer(at(field(n,"children"),1)),mn,mx,out);
 }
 // Port of the recovered Genesis convex-leaf sweep; original evidence in evidence/collision.ts.
 bool trace(V start,V delta,V mn,V mx,Hit& best){
 V end=start+delta,bmn,bmx;for(int i=0;i<3;i++){bmn[i]=std::min(start[i]+mn[i],end[i]+mn[i])-1;bmx[i]=std::max(start[i]+mx[i],end[i]+mx[i])+1;}
 auto models=field(data,"models");bool found=false;
 for(int m=0;m<count(models);m++){
 auto model=at(models,m);int mi=integer(field(model,"index"));if(disabled.count(mi))continue;
 V origin=vec(field(model,"origin"));float angle=angles[mi];V lo=vec(field(model,"mins")),hi=vec(field(model,"maxs"));
 if(angle!=0||poses.count(mi)){V a{1e9,1e9,1e9},b{-1e9,-1e9,-1e9};for(int bits=0;bits<8;bits++){V corner;for(int i=0;i<3;i++)corner[i]=(bits&(1<<i))?hi[i]:lo[i];corner=transform(corner,origin,mi,false);for(int i=0;i<3;i++){a[i]=std::min(a[i],corner[i]);b[i]=std::max(b[i],corner[i]);}}lo=a;hi=b;}
 bool overlap=true;for(int i=0;i<3;i++)if(bmx[i]<lo[i]||bmn[i]>hi[i])overlap=false;if(!overlap)continue;
 V localStart=transform(start,origin,mi,true),localEnd=transform(end,origin,mi,true),localMin,localMax;
 for(int i=0;i<3;i++){localMin[i]=std::min(localStart[i]+mn[i],localEnd[i]+mn[i])-1;localMax[i]=std::max(localStart[i]+mx[i],localEnd[i]+mx[i])+1;}
 std::vector<int> ids;leaves(integer(field(model,"root")),localMin,localMax,ids);
 Hit modelBest;modelBest.distance=9999;bool modelFound=false;
 for(int id:ids){auto leaf=at(field(data,"leafs"),id);int ns=integer(field(leaf,"num_sides"));if(!(integer(field(leaf,"contents"))&integer(field(data,"contents_mask")))||!ns)continue;
 bool committed=false;float previousDistance=modelBest.distance;
 std::function<bool(V,V,int,bool)> visit=[&](V front,V back,int side,bool possible){
 if(!possible)return false;if(side>=ns)return true;
 auto ls=at(field(data,"leaf_sides"),integer(field(leaf,"first_side"))+side);auto plane=at(field(data,"planes"),integer(field(ls,"plane")));float sign=integer(field(ls,"side"))?-1:1;V normal=vec(field(plane,"normal"))*sign;float dist=num(field(plane,"distance"))*sign;
 for(int i=0;i<3;i++)dist=float(double(dist)-double(normal[i])*(normal[i]>0?mn[i]:mx[i]));
 auto pd=[&](V p){return (double)normal.z*p.z+(double)normal.y*p.y+(double)normal.x*p.x-dist;};double a=pd(front),b=pd(back);
 if(a>=0&&b>=0)return visit(front,back,side+1,false);
 if(a<0&&b<0)return visit(front,back,side+1,true);
 bool inside=a<0;double ratio=std::clamp((a+(inside?.1:-.1))/(a-b),0.0,1.0);V split;for(int i=0;i<3;i++)split[i]=float(front[i]+ratio*(double(back[i])-front[i]));
 if(visit(front,split,side+1,inside))return true;
 if(visit(split,back,side+1,!inside)){if(!committed){V worldPoint=transform(split,origin,mi,false);V difference=worldPoint-start;double distance=std::sqrt(double(difference.z)*difference.z+double(difference.y)*difference.y+double(difference.x)*difference.x);if(distance<previousDistance){modelBest={worldPoint,transformNormal(normal,mi),float(distance),mi};modelFound=true;committed=true;}}return true;}return false;
 };visit(localStart,localEnd,0,true);
 }
 if(modelFound){V difference=modelBest.point-start;double distance=std::sqrt(double(difference.z)*difference.z+double(difference.y)*difference.y+double(difference.x)*difference.x);if(distance<best.distance){best=modelBest;found=true;}}
 }return found;
 }
 // Genesis.dll 10038470 / 100363F0: null-hull BSP point trace.
 // Global contact survives traces that enter solid without crossing a plane.
 V rayPoint{},rayNormal{};bool raySide=false;
 bool tracePoint(V start,V end,int contents,int flags,Hit& best){
 if(!(flags&2))return false;bool found=false;auto models=field(data,"models");
 for(int m=0;m<count(models);m++){
 auto model=at(models,m);int mi=integer(field(model,"index"));if(disabled.count(mi))continue;
 V origin=vec(field(model,"origin")),lo=vec(field(model,"mins")),hi=vec(field(model,"maxs"));
 if(angles[mi]!=0||poses.count(mi)){V a{1e9,1e9,1e9},b{-1e9,-1e9,-1e9};for(int bits=0;bits<8;bits++){V v;for(int i=0;i<3;i++)v[i]=(bits&(1<<i))?hi[i]:lo[i];v=transform(v,origin,mi,false);for(int i=0;i<3;i++){a[i]=std::min(a[i],v[i]);b[i]=std::max(b[i],v[i]);}}lo=a;hi=b;}
 bool overlap=true;for(int i=0;i<3;i++)if(std::max(start[i],end[i])+2<lo[i]||std::min(start[i],end[i])-2>hi[i])overlap=false;if(!overlap)continue;
 bool set=false;std::function<bool(V,V,int)> visit=[&](V front,V back,int index){
 if(index<0)return bool(integer(field(at(field(data,"leafs"),-index-1),"contents"))&contents);
 auto node=at(field(data,"nodes"),index),plane=at(field(data,"planes"),integer(field(node,"plane")));V normal=vec(field(plane,"normal"));int type=integer(field(plane,"type"));double d=num(field(plane,"distance"));
 auto distance=[&](V v){return (type<3?double(v[type]):double(normal.z)*v.z+double(normal.y)*v.y+double(normal.x)*v.x)-d;};double fd=distance(front),bd=distance(back);auto children=field(node,"children");
 if(fd>=0&&bd>=0)return visit(front,back,integer(at(children,0)));if(fd<0&&bd<0)return visit(front,back,integer(at(children,1)));
 bool side=fd<0;float ratio=float(fd/(fd-bd));V split;for(int i=0;i<3;i++)split[i]=float(double(front[i])+double(ratio)*(double(back[i])-front[i]));
 if(visit(front,split,integer(at(children,side))))return true;
 if(visit(split,back,integer(at(children,!side)))){if(!set){rayPoint=split;rayNormal=normal;raySide=side;set=true;}return true;}return false;
 };
 if(visit(transform(start,origin,mi,true),transform(end,origin,mi,true),integer(field(model,"root")))){
 rayPoint=transform(rayPoint,origin,mi,false);rayNormal=transformNormal(rayNormal,mi);V v=rayPoint-start;float dist=float(std::sqrt(double(v.z)*v.z+double(v.y)*v.y+double(v.x)*v.x));if(dist<best.distance){best={rayPoint,raySide?rayNormal*-1:rayNormal,dist,mi};found=true;}}
 if(flags&8)break;
 }return found;
 }
 bool solid(V p,V mn,V mx){std::vector<int> ids;auto models=field(data,"models");for(int m=0;m<count(models);m++){auto model=at(models,m);if(disabled.count(integer(field(model,"index"))))continue;V point=transform(p,vec(field(model,"origin")),integer(field(model,"index")),true);ids.clear();leaves(integer(field(model,"root")),point+mn,point+mx,ids);for(int id:ids){auto leaf=at(field(data,"leafs"),id);int ns=integer(field(leaf,"num_sides"));if(!(integer(field(leaf,"contents"))&integer(field(data,"contents_mask")))||!ns)continue;bool inside=true;for(int s=0;s<ns;s++){auto ls=at(field(data,"leaf_sides"),integer(field(leaf,"first_side"))+s);auto plane=at(field(data,"planes"),integer(field(ls,"plane")));float sign=integer(field(ls,"side"))?-1:1;V n=vec(field(plane,"normal"))*sign;float d=num(field(plane,"distance"))*sign;for(int i=0;i<3;i++)d-=n[i]*(n[i]>0?mn[i]:mx[i]);if(dot(n,point)-d>=0){inside=false;break;}}if(inside)return true;}}return false;}
};
struct Player{
 V p{-1268,-172,4692},v{},delta{};bool ground=false,crouch=false,climbing=false;float cooldown=0,ashDensity=1.f;
 // 00451870 stores setrescuebot at Player+0x32e4; unset is retained until
 // the authored command runs, so an unrecovered constructor default is not assumed.
 std::optional<bool> rescueBotEnabled;
 // Player init 0044D550 seeds Player+0x5b0 to 1; 0044BC90 -> 00446350 clamps [0,1].
 void setAshDensity(float value){if(value>1.f)value=1.f;if(value<0.f)value=0.f;ashDensity=value;}
 V maxs()const{return {30,crouch?25.f:85.f,30};}
 // PlayerRescue Think -> IntegrateVelocity 0x0049DBC0 -> collision callback 0x0043F810.
 void step(float dt,float forward,float strafe,float yaw,bool jump,bool jumpEdge,bool crouchEdge,Collision& c,float pitch=0){
 cooldown=std::max(0.f,cooldown-dt);if(crouchEdge){if(crouch){if(!c.solid(p,{-30,-30,-30},{30,85,30}))crouch=false;}else if(ground)crouch=true;}
 float speed=crouch?260:400;float cp=climbing?std::cos(pitch):1,sp=climbing?std::sin(pitch):0;V wish{-std::sin(yaw)*cp*forward+std::cos(yaw)*strafe,sp*forward,-std::cos(yaw)*cp*forward-std::sin(yaw)*strafe};float len=length(wish);if(len>0){wish=wish*(speed/std::max(1.f,len));double weight=std::pow((ground||climbing)?.2:.95,27*dt);v.x=float(wish.x*(1-weight)+v.x*weight);v.z=float(wish.z*(1-weight)+v.z*weight);if(climbing)v.y=float(wish.y*(1-weight)+v.y*weight);}
 bool wasClimbing=climbing;float drag=(ground||climbing)?1000:0;if(jump){if(crouch){if(jumpEdge&&!c.solid(p,{-30,-30,-30},{30,85,30}))crouch=false;delta={};return;}if((ground||climbing)&&cooldown==0){if(climbing){climbing=false;v.y=400;}else v.y+=400;cooldown=.5;}}
 V acceleration{0,wasClimbing?0.f:-1000.f,0};for(int i=0;i<3;i++){float amount=(i==1&&!wasClimbing?0:drag)*dt;float after=v[i]>0?std::max(0.f,v[i]-amount):std::min(0.f,v[i]+amount);v[i]=after+acceleration[i]*dt;delta[i]=v[i]*dt+(.5f*acceleration[i])*(dt*dt);}ground=false;
 V clipped=acceleration;bool stepped=false;
 for(int attempt=0;attempt<5;attempt++){Hit h;if(!c.trace(p,delta,{-30,-30,-30},maxs(),h))break;bool didStep=false;
 if(!stepped&&h.normal.y<=.9){Hit t;V up{0,16,0},horizontal{delta.x,0,delta.z};if(!c.trace(p,up,{-30,-30,-30},maxs(),t)&&!c.trace(p+up,horizontal,{-30,-30,-30},maxs(),t)){Hit landing;if(c.trace(p+up+horizontal,{0,-17,0},{-30,-30,-30},maxs(),landing)){p=landing.point;stepped=didStep=true;}}}
 if(!didStep){if(h.normal.y>=.5)ground=true;V inverse=h.normal*-1;float inward=dot(v,inverse);if(inward>0)v=v-inverse*inward;inward=dot(acceleration,inverse);if(inward>0)clipped=clipped-inverse*inward;delta=v*dt+clipped*(.5f*dt*dt);}
 if(attempt==4){if(h.normal.y>=.5)ground=true;else{v={};delta={};}}
 }p=p+delta;
 }
};
struct Door{
 int model;V lo,hi,pivot;float reach=100,fraction=0,direction=1;bool touching=false;
 void update(V p,float dt,Collision& c){float dist=0;for(int i=0;i<3;i++){float d=p[i]-std::clamp(p[i],lo[i],hi[i]);dist+=d*d;}bool wants=dist<=reach*reach;
 if(wants&&!touching&&fraction==0){V center=(lo+hi)*.5f;direction=(center.z-pivot.z)*(p.x-pivot.x)-(center.x-pivot.x)*(p.z-pivot.z)>0?-1:1;}touching=wants;fraction=std::clamp(fraction+(wants?1:-1)*dt*3,0.f,1.f);
 float omega=std::acos(.707107f),scale0=std::sin((1-fraction)*omega)/std::sin(omega),scale1=std::sin(fraction*omega)/std::sin(omega);float qw=scale0+scale1*.707107f,qy=scale1*.707107f*direction;c.angles[model]=2*std::atan2(qy,qw);
 }
};
