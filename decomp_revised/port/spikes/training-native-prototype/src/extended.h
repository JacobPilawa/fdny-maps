#pragma once
// Finite binary floating-point operations with the x87 64-bit significand.
// Every operation rounds to nearest, ties to even; no host long-double dependency.
struct Extended {
 using U=__uint128_t;
 uint64_t m=0;int e=0;bool negative=false; // value = signed m * 2^e
 static int width(U n){int w=0;while(n){n>>=1;w++;}return w;}
 static Extended pack(U n,int exponent,bool sign=false){
  if(!n)return {0,0,sign};int shift=width(n)-64;
  if(shift>0){U q=n>>shift,mask=(U(1)<<shift)-1,r=n&mask,half=U(1)<<(shift-1);if(r>half||(r==half&&(q&1)))q++;n=q;exponent+=shift;if(width(n)>64){n>>=1;exponent++;}}
  else if(shift<0){n<<=-shift;exponent+=shift;}
  return {uint64_t(n),exponent,sign};
 }
 static Extended from(float f){uint32_t b;std::memcpy(&b,&f,4);int exp=(b>>23)&255;uint32_t mant=b&0x7fffff;if(exp==255)throw std::runtime_error("Nonfinite extended input");return pack(exp?mant|0x800000:mant,exp?exp-127-23:-149,b>>31);}
 static Extended from(double f){uint64_t b;std::memcpy(&b,&f,8);int exp=(b>>52)&2047;uint64_t mant=b&0xfffffffffffffULL;if(exp==2047)throw std::runtime_error("Nonfinite extended input");return pack(exp?mant|0x10000000000000ULL:mant,exp?exp-1023-52:-1074,b>>63);}
 double rounded(int digits)const{if(!m)return negative?-0.0:0.0;int shift=64-digits;U q=U(m)>>shift,r=U(m)&((U(1)<<shift)-1),half=U(1)<<(shift-1);if(r>half||(r==half&&(q&1)))q++;double v=std::ldexp(double(q),e+shift);return negative?-v:v;}
 double asDouble()const{return rounded(53);}
 float asFloat()const{return float(rounded(24));}
 static Extended integer(uint32_t n){return pack(n,0);}
 static U sticky(U n,int shift){if(shift<=0)return n;if(shift>=128)return n?1:0;U mask=(U(1)<<shift)-1;return (n>>shift)|bool(n&mask);}
 Extended operator+(Extended b)const{
  if(!m&&!b.m)return {0,0,negative&&b.negative};
  if(!m)return b;if(!b.m)return *this;Extended a=*this;if(a.e<b.e)std::swap(a,b);
  U x=U(a.m)<<3,y=sticky(U(b.m)<<3,a.e-b.e);bool sign=a.negative;U n;
  if(a.negative==b.negative)n=x+y;else if(x>=y)n=x-y;else{n=y-x;sign=b.negative;}
  // Exact cancellation is +0 under the source round-to-nearest mode.
  return pack(n,a.e-3,n?sign:false);
 }
 Extended operator-()const{Extended n=*this;n.negative=!negative;return n;}
 Extended operator-(Extended b)const{return *this+-b;}
 Extended operator*(Extended b)const{return pack(U(m)*b.m,e+b.e,negative!=b.negative);}
 Extended operator/(Extended b)const{
  if(!b.m)throw std::runtime_error("Extended division by zero");if(!m)return {0,0,negative!=b.negative};
  int shift=m<b.m?64:63;U numerator=U(m)<<shift,q=numerator/b.m,r=numerator%b.m;
  if(r*2>b.m||(r*2==b.m&&(q&1)))q++;
  return pack(q,e-b.e-shift,negative!=b.negative);
 }
 int64_t truncate()const{if(!m)return 0;U n=e>=0?U(m)<<e:(-e>=128?0:U(m)>>-e);return negative?-int64_t(n):int64_t(n);}
};
