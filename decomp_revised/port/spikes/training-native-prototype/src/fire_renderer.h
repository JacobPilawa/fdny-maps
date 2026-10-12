#pragma once
// Original texture IDs (rescue.rrf), constructor UVs (0047F850),
// geometry (0044A840), frame timing/opacity and shipped driver blend state.
struct FireRenderer {
 std::map<int,GLuint> textures;
 static constexpr float uv[4][2]={{0,.01f},{.99f,.01f},{.99f,.99f},{0,.99f}};
 void load(){
  for(int group=0;group<2;group++)for(int frame=0;frame<19;frame++){
   char filename[64];snprintf(filename,sizeof(filename),group?"fflame%04d.bmp":"lflame-%02d.bmp",frame);
   auto path=assetDirectory()+"/fire-sprites/"+filename;auto surface=SDL_LoadBMP(path.c_str());if(!surface)throw std::runtime_error("Cannot load fire image "+path);
   auto rgba=SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_ABGR8888,0);SDL_FreeSurface(surface);if(!rgba)throw std::runtime_error(SDL_GetError());
   GLuint texture;glGenTextures(1,&texture);glBindTexture(GL_TEXTURE_2D,texture);glPixelStorei(GL_UNPACK_ALIGNMENT,4);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,rgba->w,rgba->h,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba->pixels);SDL_FreeSurface(rgba);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);textures[(group?120:20)+frame]=texture;
  }
 }
 int draw(FireEntity& fire,uint32_t now,bool visible){
  auto& visual=fire.visual;if(!visual.enabled||!visible)return 0;visual.renderStep(now,true);
  glPushAttrib(GL_ENABLE_BIT|GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_TEXTURE_BIT|GL_CURRENT_BIT);
  FireRenderState::fromDriverFlags(0xa0).apply();glEnable(GL_TEXTURE_2D);int drawn=0;
  for(int sprite=0;sprite<4;sprite++){
   auto& animation=visual.animations[sprite];if(!animation.active)continue;
   auto opacity=FireVisual::opacityBytes(now-animation.started,animation.rate,sprite==3?fire.glowAlpha:fire.alpha);
   glBindTexture(GL_TEXTURE_2D,textures.at(visual.textureId(sprite)));glColor4ub(255,255,255,opacity[0]);
   glBegin(GL_QUADS);for(int v=0;v<4;v++){glTexCoord2f(uv[v][0],uv[v][1]);auto p=visual.placement.sprites[sprite].vertices[v];glVertex3f(p.x,p.y,p.z);}glEnd();drawn++;
  }
  glPopAttrib();return drawn;
 }
 void release(){for(auto& entry:textures)glDeleteTextures(1,&entry.second);textures.clear();}
};
