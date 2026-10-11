#pragma once
// 0049DA00 places a model hull on the floor. The original adds 48 before
// tracing 1,000 downward, then stores the hit position with Y raised by 1.
// A failed trace logs a placement error and still stores the zero-initialized
// result plus that final offset. The callback supplies world collision.
inline bool placeModelOnFloor(V& position,V mins,V maxs,
 const std::function<bool(V,V,V,V,V&)>& trace,
 const std::function<void()>& reportFailure){
 position.y=float(double(position.y)+48.0);
 V point{};
 bool found=trace(position,{0,-1000,0},mins,maxs,point);
 if(!found)reportFailure();
 position=point;position.y=float(double(position.y)+1.0);
 return found;
}
