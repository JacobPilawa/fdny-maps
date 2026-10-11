#pragma once
// 00461F80: copy the tool world pose, rotate its Y/Z basis by -90
// degrees (00496B50), offset the emitters, then sample at 0/60/120/180.
struct ExtinguisherSamples {
 static V normalized(V v){double magnitude=std::sqrt(double(v.z)*v.z+double(v.y)*v.y+double(v.x)*v.x);return {float(v.x/magnitude),float(v.y/magnitude),float(v.z/magnitude)};}
 static std::array<V,4> points(const float* matrix,V origin){
  V x{matrix[0],matrix[3],matrix[6]},y{matrix[1],matrix[4],matrix[7]},z{matrix[2],matrix[5],matrix[8]};
  double angle=-90.0*double(.01745329238474369f),c=std::cos(angle),s=std::sin(angle);
  V rotatedY;for(int a=0;a<3;a++)rotatedY[a]=float(c*y[a]-s*z[a]);
  y=normalized(rotatedY);z=normalized(FireFloor::cross(x,y));
  origin=origin+z*20;origin=origin+y*10;origin=origin+z*10;
  std::array<V,4> result;for(int i=0;i<4;i++)result[i]=origin+z*float(i*60);return result;
 }
};
