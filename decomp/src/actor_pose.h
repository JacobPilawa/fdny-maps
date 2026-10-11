#pragma once
// Genesis Actor/pose.c: world joint = parent * attachment * motion key.
// Actor vertices are stored in their owning bone's local coordinates.
struct ActorTransform {
 float r[9]={1,0,0,0,1,0,0,0,1};V t{};
 V rotate(V p)const{return {float(double(r[0])*p.x+double(r[1])*p.y+double(r[2])*p.z),float(double(r[3])*p.x+double(r[4])*p.y+double(r[5])*p.z),float(double(r[6])*p.x+double(r[7])*p.y+double(r[8])*p.z)};}
 V point(V p)const{return {float(double(r[0])*p.x+double(r[1])*p.y+double(r[2])*p.z+t.x),float(double(r[3])*p.x+double(r[4])*p.y+double(r[5])*p.z+t.y),float(double(r[6])*p.x+double(r[7])*p.y+double(r[8])*p.z+t.z)};}
};
inline ActorTransform actorMultiply(const ActorTransform& a,const ActorTransform& b){
 ActorTransform result;for(int row=0;row<3;row++)for(int col=0;col<3;col++)result.r[row*3+col]=float(double(a.r[row*3])*b.r[col]+double(a.r[row*3+1])*b.r[col+3]+double(a.r[row*3+2])*b.r[col+6]);result.t=a.point(b.t);return result;
}
// VictimRescueEnt builds the actor-local frame in 0045B690 and parents it to
// the floor-adjusted entity frame in 0045D5B0. 004977B0/00497300 compose
// parent * child, while 00495D20 applies child scale before the parent basis.
// Training headings are all 6, converted by the original caller to 360 deg.
// This native trigonometry reproduces the transform geometry; x87 residual
// bits from the original helpers are outside this renderer-level path.
inline ActorTransform victimActorRoot(const VictimSpawn& spawn){
 const double degrees=double(spawn.selectedHeading)*30.0+180.0;
 const double angle=degrees*double(float(0.01745329238474369f));
 const float c=float(std::cos(angle)),s=float(std::sin(angle));
 ActorTransform parent;parent.r[0]=c;parent.r[1]=0;parent.r[2]=-s;parent.r[3]=0;parent.r[4]=1;parent.r[5]=0;parent.r[6]=s;parent.r[7]=0;parent.r[8]=c;parent.t=spawn.selectedPosition;
 // 00496B50 turns local Y toward -Z, then rebuilds Z and X by cross product.
 ActorTransform local;local.r[0]=1;local.r[1]=0;local.r[2]=0;local.r[3]=0;local.r[4]=0;local.r[5]=1;local.r[6]=0;local.r[7]=-1;local.r[8]=0;local.t={0,-25,0};
 ActorTransform result=actorMultiply(parent,local);
 // Actor instance scale is stored separately at +0x34 and composes to 2.
 for(float& axis:result.r)axis*=2.f;
 return result;
}
inline ActorTransform actorAttachment(J bone){ActorTransform result;auto matrix=field(bone,"attachment");if(count(matrix)!=12)throw std::runtime_error("Invalid actor bone attachment");for(int i=0;i<9;i++)result.r[i]=num(at(matrix,i));result.t={num(at(matrix,9)),num(at(matrix,10)),num(at(matrix,11))};return result;}
inline ActorTransform actorKey(Pose pose){
 // Genesis.dll 1002B970 retains XX, XY and ZW in x87 registers while
 // rounding the other products into six float stack slots.
 Q q=pose.rotation;double x2=2.0*double(q.x),y2=2.0*double(q.y),z2=2.0*double(q.z);
 double xx=x2*q.x,xy=x2*q.y,zw=z2*q.w;
 float xz=float(x2*q.z),xw=float(x2*q.w),yy=float(y2*q.y),yz=float(y2*q.z),yw=float(y2*q.w),zz=float(z2*q.z);
 ActorTransform result;float rows[9]={float(1.0-double(yy)-zz),float(xy-zw),float(double(yw)+xz),float(zw+xy),float(1.0-xx-zz),float(double(yz)-xw),float(double(xz)-yw),float(double(yz)+xw),float(1.0-xx-yy)};
 std::copy(rows,rows+9,result.r);result.t=pose.translation;return result;
}

struct ActorPose {
 J asset;std::vector<std::map<std::string,J>> paths;
 explicit ActorPose(J source):asset(source){
  auto motions=field(asset,"motions");
  for(int i=0;i<count(motions);i++){
   std::map<std::string,J> tracks;auto sourcePaths=field(at(motions,i),"paths");
   for(int k=0;k<count(sourcePaths);k++){
    auto path=at(sourcePaths,k);J normalized=json_object_new_object();
    for(auto channel:{"translation","rotation"}){
     auto sourceTrack=field(path,channel);if(!sourceTrack)continue;
     int mode=integer(field(sourceTrack,"interpolation"));
     if(std::string(channel)=="rotation"&&mode!=1)throw std::runtime_error("Unrecovered actor rotation interpolation");
     if(std::string(channel)=="translation"&&mode!=0&&mode!=1&&mode!=2)throw std::runtime_error("Unrecovered actor translation interpolation");
     J track=json_object_new_object(),keys=json_object_new_array(),format=json_object_new_array();
     json_object_array_add(format,json_object_new_int(0));json_object_array_add(format,json_object_new_int(mode));json_object_object_add(track,"format",format);
     auto times=field(sourceTrack,"times"),values=field(sourceTrack,"values");if(count(times)!=count(values))throw std::runtime_error("Actor key/value counts disagree");
     for(int j=0;j<count(times);j++){J key=json_object_new_array();json_object_array_add(key,json_object_get(at(times,j)));auto value=at(values,j);for(int axis=0;axis<count(value);axis++)json_object_array_add(key,json_object_get(at(value,axis)));json_object_array_add(keys,key);}
     json_object_object_add(track,"keys",keys);json_object_object_add(normalized,channel,track);
    }
    tracks[str(field(path,"name"))]=normalized;
   }
   paths.push_back(std::move(tracks));
  }
 }
 ~ActorPose(){for(auto& motion:paths)for(auto& pair:motion)json_object_put(pair.second);}
 ActorPose(const ActorPose&)=delete;ActorPose& operator=(const ActorPose&)=delete;
 std::vector<ActorTransform> joints(int motion,float time)const{
  if(motion>=int(paths.size())||motion< -1)throw std::runtime_error("Invalid actor motion");
  auto bones=field(asset,"bones");std::vector<ActorTransform> result(count(bones));
  for(int i=0;i<count(bones);i++){
   auto bone=at(bones,i);ActorTransform local=actorAttachment(bone);
   if(motion>=0){auto found=paths[motion].find(str(field(bone,"name")));if(found!=paths[motion].end())local=actorMultiply(local,actorKey(sampleMotion(found->second,time)));}
   int parent=integer(field(bone,"parent"));if(parent>=i||parent< -1)throw std::runtime_error("Invalid actor bone hierarchy");result[i]=parent>=0?actorMultiply(result[parent],local):local;
  }
  return result;
 }
 std::vector<V> vertices(int motion,float time)const{
  auto transforms=joints(motion,time);auto source=field(asset,"vertices");std::vector<V> result;result.reserve(count(source));
  for(int i=0;i<count(source);i++){auto vertex=at(source,i);int bone=integer(field(vertex,"bone"));if(bone<0||bone>=int(transforms.size()))throw std::runtime_error("Invalid actor vertex bone");result.push_back(transforms[bone].point(vec(field(vertex,"position"))));}return result;
 }
};
