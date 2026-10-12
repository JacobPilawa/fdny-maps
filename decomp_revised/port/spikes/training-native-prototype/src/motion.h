#pragma once
struct Q{float w=1,x=0,y=0,z=0;};
inline Q qkey(J key){return {num(at(key,1)),num(at(key,2)),num(at(key,3)),num(at(key,4))};}
inline Q normalizeQuaternion(Q q){float length=std::sqrt(q.w*q.w+q.x*q.x+q.y*q.y+q.z*q.z);if(length!=0){q.w/=length;q.x/=length;q.y/=length;q.z/=length;}return q;}
inline Q slerp(Q a,Q b,float t){
 // Genesis geQuaternion_SlerpNotShortest. No shortest-path sign flip.
 float cosine=a.w*b.w+a.x*b.x+a.y*b.y+a.z*b.z,scale0,scale1;
 if(1+cosine>0.00001){if(1-cosine>0.00001){float omega=std::acos(cosine),sine=std::sin(omega);if(sine<.00001){scale0=1-t;scale1=t;}else{scale0=std::sin((1-t)*omega)/sine;scale1=std::sin(t*omega)/sine;}}else{scale0=1-t;scale1=t;}}
 else{b={a.z,-a.y,a.x,-a.w};scale0=std::sin((1-t)*3.141592653589793*.5);scale1=std::sin(t*3.141592653589793*.5);}
 return {scale0*a.w+scale1*b.w,scale0*a.x+scale1*b.x,scale0*a.y+scale1*b.y,scale0*a.z+scale1*b.z};
}
struct Pose{V translation{};Q rotation{};};
inline std::pair<int,float> bracket(J keys,float time){int n=count(keys);if(n<2||time<=num(at(at(keys,0),0)))return {0,0};if(time>=num(at(at(keys,n-1),0)))return {n-1,0};int i=0;while(i+1<n&&num(at(at(keys,i+1),0))<=time)i++;float a=num(at(at(keys,i),0)),b=num(at(at(keys,i+1),0));return {i,(time-a)/(b-a)};}
inline V vkey(J key){return {num(at(key,1)),num(at(key,2)),num(at(key,3))};}
inline V derivative(J keys,int i,bool outgoing,bool zero){if(zero)return {};int n=count(keys);if(n<2)return {};if(i==0)return vkey(at(keys,1))-vkey(at(keys,0));if(i==n-1)return vkey(at(keys,i))-vkey(at(keys,i-1));float n0=num(at(at(keys,i),0))-num(at(at(keys,i-1),0)),n1=num(at(at(keys,i+1),0))-num(at(at(keys,i),0));return (vkey(at(keys,i+1))-vkey(at(keys,i-1)))*((outgoing?n1:n0)/(n0+n1));}
inline Pose sampleMotion(J motion,float time){Pose pose;auto track=field(motion,"translation"),keys=field(track,"keys");if(count(keys)){auto [i,t]=bracket(keys,time);V a=vkey(at(keys,i));if(i+1>=count(keys)||t==0)pose.translation=a;else{V b=vkey(at(keys,i+1));int mode=integer(at(field(track,"format"),1));if(mode==0)pose.translation=a+(b-a)*t;else{float t2=t*t,t3=t2*t,h2=-(t3+t3)+t2*3,h1=1-h2,h4=t3-t2,h3=h4-t2+t;pose.translation=a*h1+b*h2+derivative(keys,i,true,mode==2)*h3+derivative(keys,i+1,false,mode==2)*h4;}}}
 keys=field(field(motion,"rotation"),"keys");if(count(keys)){auto [i,t]=bracket(keys,time);Q a=qkey(at(keys,i));pose.rotation=i+1>=count(keys)||t==0?a:slerp(a,qkey(at(keys,i+1)),t);}return pose;}
inline float motionDuration(J motion){float end=0;for(auto channel:{"rotation","translation"}){auto keys=field(field(motion,channel),"keys");if(count(keys))end=std::max(end,num(at(at(keys,count(keys)-1),0)));}return end;}
inline V rotateQ(V v,Q q){
 // geQuaternion_ToMatrix applied to a vector.
 float xs=q.x+q.x,ys=q.y+q.y,zs=q.z+q.z,wx=q.w*xs,wy=q.w*ys,wz=q.w*zs,xx=q.x*xs,xy=q.x*ys,xz=q.x*zs,yy=q.y*ys,yz=q.y*zs,zz=q.z*zs;
 return {(1-yy-zz)*v.x+(xy-wz)*v.y+(xz+wy)*v.z,(xy+wz)*v.x+(1-xx-zz)*v.y+(yz-wx)*v.z,(xz-wy)*v.x+(yz+wx)*v.y+(1-xx-yy)*v.z};
}
