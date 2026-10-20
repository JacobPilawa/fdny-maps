#pragma once
#include <map>
#include <optional>
#include <set>
struct V{float x=0,y=0,z=0;float& operator[](int i){return i==0?x:i==1?y:z;} float operator[](int i)const{return i==0?x:i==1?y:z;}};
V vec(J j){return {num(at(j,0)),num(at(j,1)),num(at(j,2))};}
V operator+(V a,V b){return {a.x+b.x,a.y+b.y,a.z+b.z};} V operator-(V a,V b){return {a.x-b.x,a.y-b.y,a.z-b.z};} V operator*(V a,float f){return {a.x*f,a.y*f,a.z*f};}
float dot(V a,V b){return float((double)a.x*b.x+(double)a.y*b.y+(double)a.z*b.z);} float length(V a){return std::sqrt(dot(a,a));}
#include "motion.h"
struct Hit{V point,normal;float distance=99999;int model=-1;};
struct CollisionModelBounds{int index=0,root=0;size_t sourceOrder=0;V origin{},mins{},maxs{};};
struct Collision{
 J data;std::set<int> disabled;std::set<int> bmodelStateCollisionDisabled;std::map<int,float> angles;std::map<int,Pose> poses;std::map<int,V> modelOrigins;std::vector<CollisionModelBounds> collisionModels;std::vector<const CollisionModelBounds*> staticModelsByMinX,dynamicModels,candidateScratch;std::set<int> dynamicModelIndices;
 bool modelCollisionDisabled(int model)const{return disabled.count(model)!=0||bmodelStateCollisionDisabled.count(model)!=0;}
 float modelAngle(int model){auto it=angles.find(model);return it==angles.end()?0.f:it->second;}
 V rotate(V p,V origin,float a){if(a==0)return p;float c=std::cos(a),sn=std::sin(a);V v=p-origin;return origin+V{c*v.x+sn*v.z,v.y,-sn*v.x+c*v.z};}
 V transform(V p,V origin,int model,bool inverse){auto it=poses.find(model);if(it==poses.end())return rotate(p,origin,inverse?-modelAngle(model):modelAngle(model));Pose pose=it->second;if(inverse){p=p-pose.translation;pose.rotation.x=-pose.rotation.x;pose.rotation.y=-pose.rotation.y;pose.rotation.z=-pose.rotation.z;}V result=rotateQ(p-origin,pose.rotation)+origin;return inverse?result:result+pose.translation;}
 V transformNormal(V n,int model){auto it=poses.find(model);return it==poses.end()?rotate(n,{},modelAngle(model)):rotateQ(n,it->second.rotation);}
 explicit Collision(J d):data(d){auto sourceModels=field(data,"models");collisionModels.reserve(count(sourceModels));for(int i=0;i<count(sourceModels);i++){auto source=at(sourceModels,i);CollisionModelBounds model;model.index=integer(field(source,"index"));model.root=integer(field(source,"root"));model.sourceOrder=size_t(i);model.origin=vec(field(source,"origin"));model.mins=vec(field(source,"mins"));model.maxs=vec(field(source,"maxs"));modelOrigins[model.index]=model.origin;collisionModels.push_back(model);}staticModelsByMinX.reserve(collisionModels.size());candidateScratch.reserve(collisionModels.size());for(const auto& model:collisionModels)staticModelsByMinX.push_back(&model);std::sort(staticModelsByMinX.begin(),staticModelsByMinX.end(),[](const auto* a,const auto* b){return a->mins.x==b->mins.x?a->sourceOrder<b->sourceOrder:a->mins.x<b->mins.x;});}
 void setPose(int model,Pose pose){poses[model]=pose;if(!dynamicModelIndices.insert(model).second)return;auto found=std::find_if(collisionModels.begin(),collisionModels.end(),[&](const auto& candidate){return candidate.index==model;});if(found==collisionModels.end())return;dynamicModels.push_back(&*found);}
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
 bool found=false;
 auto traceModel=[&](const CollisionModelBounds& model){int mi=model.index;if(modelCollisionDisabled(mi))return;
 V origin=model.origin;float angle=modelAngle(mi);V lo=model.mins,hi=model.maxs;
 if(angle!=0||poses.count(mi)){V a{1e9,1e9,1e9},b{-1e9,-1e9,-1e9};for(int bits=0;bits<8;bits++){V corner;for(int i=0;i<3;i++)corner[i]=(bits&(1<<i))?hi[i]:lo[i];corner=transform(corner,origin,mi,false);for(int i=0;i<3;i++){a[i]=std::min(a[i],corner[i]);b[i]=std::max(b[i],corner[i]);}}lo=a;hi=b;}
 bool overlap=true;for(int i=0;i<3;i++)if(bmx[i]<lo[i]||bmn[i]>hi[i])overlap=false;if(!overlap)return;
 V localStart=transform(start,origin,mi,true),localEnd=transform(end,origin,mi,true),localMin,localMax;
 for(int i=0;i<3;i++){localMin[i]=std::min(localStart[i]+mn[i],localEnd[i]+mn[i])-1;localMax[i]=std::max(localStart[i]+mx[i],localEnd[i]+mx[i])+1;}
 std::vector<int> ids;leaves(model.root,localMin,localMax,ids);
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
 };
 candidateScratch.clear();for(const auto* model:staticModelsByMinX){if(model->mins.x>bmx.x)break;if(dynamicModelIndices.count(model->index))continue;if(model->maxs.x<bmn.x||model->maxs.y<bmn.y||model->mins.y>bmx.y||model->maxs.z<bmn.z||model->mins.z>bmx.z)continue;candidateScratch.push_back(model);}for(const auto* model:dynamicModels)candidateScratch.push_back(model);std::sort(candidateScratch.begin(),candidateScratch.end(),[](const auto* a,const auto* b){return a->sourceOrder<b->sourceOrder;});for(const auto* model:candidateScratch)traceModel(*model);return found;
 }
 // Genesis.dll 10038470 / 100363F0: null-hull BSP point trace.
 // Global contact survives traces that enter solid without crossing a plane.
 V rayPoint{},rayNormal{};bool raySide=false;
 bool tracePoint(V start,V end,int contents,int flags,Hit& best){
 if(!(flags&2))return false;bool found=false;
 for(const auto& model:collisionModels){int mi=model.index;if(modelCollisionDisabled(mi))continue;
 V origin=model.origin,lo=model.mins,hi=model.maxs;
 if(modelAngle(mi)!=0||poses.count(mi)){V a{1e9,1e9,1e9},b{-1e9,-1e9,-1e9};for(int bits=0;bits<8;bits++){V v;for(int i=0;i<3;i++)v[i]=(bits&(1<<i))?hi[i]:lo[i];v=transform(v,origin,mi,false);for(int i=0;i<3;i++){a[i]=std::min(a[i],v[i]);b[i]=std::max(b[i],v[i]);}}lo=a;hi=b;}
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
 if(visit(transform(start,origin,mi,true),transform(end,origin,mi,true),model.root)){
 rayPoint=transform(rayPoint,origin,mi,false);rayNormal=transformNormal(rayNormal,mi);V v=rayPoint-start;float dist=float(std::sqrt(double(v.z)*v.z+double(v.y)*v.y+double(v.x)*v.x));if(dist<best.distance){best={rayPoint,raySide?rayNormal*-1:rayNormal,dist,mi};found=true;}}
 if(flags&8)break;
 }return found;
 }
 // PlayerHose's WindowFire callback tests the stream impact against each
 // target model's AABB expanded by the shared component contact radius.
 bool modelBoundsOverlapPoint(int modelIndex,V point,float reach){
 const auto found=std::find_if(collisionModels.begin(),collisionModels.end(),[&](const auto& model){return model.index==modelIndex;});
 if(found==collisionModels.end())return false;
 V lo=found->mins,hi=found->maxs;
 if(modelAngle(modelIndex)!=0||poses.count(modelIndex)){V a{1e9f,1e9f,1e9f},b{-1e9f,-1e9f,-1e9f};for(int bits=0;bits<8;bits++){V corner;for(int axis=0;axis<3;axis++)corner[axis]=(bits&(1<<axis))?hi[axis]:lo[axis];corner=transform(corner,found->origin,modelIndex,false);for(int axis=0;axis<3;axis++){a[axis]=std::min(a[axis],corner[axis]);b[axis]=std::max(b[axis],corner[axis]);}}lo=a;hi=b;}
 return point.x+reach>=lo.x&&point.x-reach<=hi.x&&point.y+reach>=lo.y&&point.y-reach<=hi.y&&point.z+reach>=lo.z&&point.z-reach<=hi.z;
 }
 bool solid(V p,V mn,V mx){std::vector<int> ids;for(const auto& model:collisionModels){if(disabled.count(model.index))continue;V point=transform(p,model.origin,model.index,true);ids.clear();leaves(model.root,point+mn,point+mx,ids);for(int id:ids){auto leaf=at(field(data,"leafs"),id);int ns=integer(field(leaf,"num_sides"));if(!(integer(field(leaf,"contents"))&integer(field(data,"contents_mask")))||!ns)continue;bool inside=true;for(int s=0;s<ns;s++){auto ls=at(field(data,"leaf_sides"),integer(field(leaf,"first_side"))+s);auto plane=at(field(data,"planes"),integer(field(ls,"plane")));float sign=integer(field(ls,"side"))?-1:1;V n=vec(field(plane,"normal"))*sign;float d=num(field(plane,"distance"))*sign;for(int i=0;i<3;i++)d-=n[i]*(n[i]>0?mn[i]:mx[i]);if(dot(n,point)-d>=0){inside=false;break;}}if(inside)return true;}}return false;}
};
struct Player{
 V p{-1268,-172,4692},v{},delta{};bool ground=false,crouch=false,climbing=false;float cooldown=0,ashDensity=1.f;
 // Retail Player fields: +0x12C is a transient upward-contact latch,
 // +0x3400 is the oxygen/burn-gated safe-position snapshot, and +0x534 is
 // the separate reset checkpoint. The retail constructor zeros the snapshot
 // and seeds the checkpoint from the starting frame.
 bool collisionLatch=false;
 V safePosition3400{},checkpointPosition{};
 float checkpointYaw=0.f;
 // 00451870 stores setrescuebot at Player+0x32e4; unset is retained until
 // the authored command runs, so an unrecovered constructor default is not assumed.
 std::optional<bool> rescueBotEnabled;
 // Player init 0044D550 seeds Player+0x5b0 to 1; 0044BC90 -> 00446350 clamps [0,1].
 void setAshDensity(float value){if(value>1.f)value=1.f;if(value<0.f)value=0.f;ashDensity=value;}
 V maxs()const{return {30,crouch?25.f:85.f,30};}
 // PlayerRescue Think -> IntegrateVelocity 0x0049DBC0 -> collision callback 0x0043F810.
 void step(float dt,float forward,float strafe,float yaw,bool jump,bool jumpEdge,bool crouchEdge,Collision& c,float pitch=0){
 collisionLatch=false;
 cooldown=std::max(0.f,cooldown-dt);if(crouchEdge){if(crouch){if(!c.solid(p,{-30,-30,-30},{30,85,30}))crouch=false;}else if(ground)crouch=true;}
 float speed=crouch?260:400;float cp=climbing?std::cos(pitch):1,sp=climbing?std::sin(pitch):0;V wish{-std::sin(yaw)*cp*forward+std::cos(yaw)*strafe,sp*forward,-std::cos(yaw)*cp*forward-std::sin(yaw)*strafe};float len=length(wish);if(len>0){wish=wish*(speed/std::max(1.f,len));double weight=std::pow((ground||climbing)?.2:.95,27*dt);v.x=float(wish.x*(1-weight)+v.x*weight);v.z=float(wish.z*(1-weight)+v.z*weight);if(climbing)v.y=float(wish.y*(1-weight)+v.y*weight);}
 bool wasClimbing=climbing;float drag=(ground||climbing)?1000:0;if(jump){if(crouch){if(jumpEdge&&!c.solid(p,{-30,-30,-30},{30,85,30}))crouch=false;delta={};return;}if((ground||climbing)&&cooldown==0){if(climbing){climbing=false;v.y=400;}else v.y+=400;cooldown=.5;}}
 V acceleration{0,wasClimbing?0.f:-1000.f,0};for(int i=0;i<3;i++){float amount=(i==1&&!wasClimbing?0:drag)*dt;float after=v[i]>0?std::max(0.f,v[i]-amount):std::min(0.f,v[i]+amount);v[i]=after+acceleration[i]*dt;delta[i]=v[i]*dt+(.5f*acceleration[i])*(dt*dt);}ground=false;
 V clipped=acceleration;bool stepped=false;
 for(int attempt=0;attempt<5;attempt++){Hit h;if(!c.trace(p,delta,{-30,-30,-30},maxs(),h))break;bool didStep=false;
 if(!stepped&&h.normal.y<=.9){Hit t;V up{0,16,0},horizontal{delta.x,0,delta.z};if(!c.trace(p,up,{-30,-30,-30},maxs(),t)&&!c.trace(p+up,horizontal,{-30,-30,-30},maxs(),t)){Hit landing;if(c.trace(p+up+horizontal,{0,-17,0},{-30,-30,-30},maxs(),landing)){p=landing.point;stepped=didStep=true;}}}
 if(!didStep){if(h.normal.y>=.5){ground=true;collisionLatch=true;}V inverse=h.normal*-1;float inward=dot(v,inverse);if(inward>0)v=v-inverse*inward;inward=dot(acceleration,inverse);if(inward>0)clipped=clipped-inverse*inward;delta=v*dt+clipped*(.5f*dt*dt);}
 if(attempt==4){if(h.normal.y>=.5)ground=true;else{v={};delta={};}}
 }p=p+delta;
 }
};
struct Door{
 int entity=0,model=0;V pivot,openAwayLo,openAwayHi;float fraction=0,timeScale=0;int motionState=0,hitPoints=-1;Q pathEndRotation{};
 void chooseOpenAway(V player){
  // RCS_BModel::openaway (0043D6A0). Training door keyframe 0 is identity,
  // so the source transform keyframe0^T * keyframe1 is pathEndRotation.
  V p0{openAwayLo.x,0,openAwayLo.z},p1{openAwayHi.x,0,openAwayHi.z};V d0=p0-pivot,d1=p1-pivot;
  V direction=length(d0)<=length(d1)?p1-p0:p0-p1;float span=length(direction);if(span!=0)direction=direction*(1/span);
  V transformed=rotateQ(direction,pathEndRotation);if(dot(transformed,player-pivot)>=0)pathEndRotation=normalizeQuaternion({pathEndRotation.w,-pathEndRotation.x,-pathEndRotation.y,-pathEndRotation.z});
 }
 void update(float dt,Collision& c){
  if(motionState==3){fraction+=dt*timeScale;if(fraction>=1.f){fraction=1.f;motionState=0;}}
  else if(motionState==4){fraction-=dt*timeScale;if(fraction<=0.f){fraction=0.f;motionState=0;}}
 Q rotation=slerp(Q{},pathEndRotation,fraction);c.angles[model]=2*std::atan2(rotation.y,rotation.w);
 }
};
