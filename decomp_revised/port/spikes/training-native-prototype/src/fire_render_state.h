#pragma once
// D3DDrv.dll 100076B0. D3D states: 27 blend enable, 19/20 factors,
// 23 depth comparison, 14 depth writes. Factors 5=SRCALPHA, 6=INVSRCALPHA, 2=ONE.
struct FireRenderState {
 bool blending,additive,depthWrite,alwaysDepth;
 static FireRenderState fromDriverFlags(uint32_t flags){return {bool(flags&0x8c),bool(flags&0x80),!(flags&0x20),bool(flags&0x10)};}
 void apply()const{
  if(blending){glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,additive?GL_ONE:GL_ONE_MINUS_SRC_ALPHA);}else glDisable(GL_BLEND);
  glDepthMask(depthWrite?GL_TRUE:GL_FALSE);glDepthFunc(alwaysDepth?GL_ALWAYS:GL_LEQUAL);
 }
};
