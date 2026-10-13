#pragma once
#include <filesystem>
#include <string>
#include <utility>
#include <stdexcept>

// First native draw path for a recovered ACT model. The caller supplies the
// model-specific bitmap directory and source-derived actor root transform.
struct VictimRenderer {
 J asset;ActorPose pose;std::filesystem::path bitmapDirectory;std::string stem;std::vector<GLuint> textures;
 VictimRenderer(J model,std::filesystem::path directory,std::string assetStem)
  :asset(model),pose(model),bitmapDirectory(std::move(directory)),stem(std::move(assetStem)){}
 VictimRenderer(const VictimRenderer&)=delete;VictimRenderer& operator=(const VictimRenderer&)=delete;
 ~VictimRenderer(){release();}
 void load(){
  auto materials=field(asset,"materials");textures.reserve(count(materials));
  try{for(int i=0;i<count(materials);i++){
   auto path=bitmapDirectory/(stem+"_"+std::to_string(i)+".bmp");
   auto surface=SDL_LoadBMP(path.string().c_str());if(!surface)throw std::runtime_error("Cannot load actor material "+path.string());
   auto rgba=SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_ABGR8888,0);SDL_FreeSurface(surface);if(!rgba)throw std::runtime_error(SDL_GetError());
   GLuint texture=0;glGenTextures(1,&texture);glBindTexture(GL_TEXTURE_2D,texture);glPixelStorei(GL_UNPACK_ALIGNMENT,4);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,rgba->w,rgba->h,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba->pixels);SDL_FreeSurface(rgba);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);textures.push_back(texture);
  }}catch(...){release();throw;}
 }
 int draw(const VictimSpawn& spawn,const ActorAnimation& animation,uint32_t now,const ActorTransform& root){
  if(!spawn.spawnSelected)return 0;
  if(spawn.appearanceIndex<0||spawn.appearanceIndex>=int(textures.size()))throw std::runtime_error("Victim appearance index is outside the ACT material table");
  float time=animation.poseTime(now);auto vertices=pose.vertices(animation.motion,time);
  auto lods=field(asset,"lods");if(count(lods)<1)throw std::runtime_error("Victim ACT has no LOD");auto faces=at(lods,0);
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT|GL_CURRENT_BIT|GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);glColor4ub(255,255,255,255);int triangles=0;
  for(int f=0;f<count(faces);f++){
   auto face=at(faces,f);int faceMaterial=integer(field(face,"material"));
   // Original 0045D5B0 replaces the actor's material slot 0 with the selected
   // appearance; other ACT slots retain their own material.
   int material=faceMaterial==0?spawn.appearanceIndex:faceMaterial;
   if(material<0||material>=int(textures.size()))throw std::runtime_error("Victim face references a missing ACT material");
   glBindTexture(GL_TEXTURE_2D,textures[size_t(material)]);auto indices=field(face,"vertices");
   glBegin(GL_TRIANGLES);
   for(int corner=0;corner<3;corner++){
    int index=integer(at(indices,corner));if(index<0||index>=int(vertices.size()))throw std::runtime_error("Victim face vertex index is invalid");
    auto source=at(field(asset,"vertices"),index);auto uv=field(source,"uv");V point=root.point(vertices[size_t(index)]);
    glTexCoord2f(num(at(uv,0)),num(at(uv,1)));glVertex3f(point.x,point.y,point.z);
   }
   glEnd();triangles++;
  }
  glPopAttrib();return triangles;
 }
 void release(){if(textures.empty())return;glDeleteTextures(GLsizei(textures.size()),textures.data());textures.clear();}
};
