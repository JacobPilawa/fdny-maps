#pragma once
// 0044A7A0: vertically compressed ellipsoid; tool adds contact padding.
struct FireContact {
 static float influence(V size,V origin,V point,float padding,float alpha){
  double base=(double(size.x)+size.z)*.5;if(base<.01)return 0;
  float radius=float(base+padding);V delta=point-origin;
  double vertical=double(delta.y)*2;
  double squared=double(delta.z)*delta.z+double(delta.x)*delta.x+vertical*vertical;
  if(squared>double(radius)*radius)return 0;
  return float((double(alpha)/radius)*(double(radius)-std::sqrt(squared)));
 }
};
