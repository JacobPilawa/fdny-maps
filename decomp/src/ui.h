#pragma once
#include <CoreText/CoreText.h>
#include <CoreGraphics/CoreGraphics.h>
#include <sstream>
// Platform UI adapter. Gameplay remains in original map coordinates.
struct TextOverlay {
 GLuint texture=0;std::string previous;int width=1100,height=160;
 void draw(const std::string& text,int viewportWidth,int viewportHeight){
 if(text.empty())return;
 if(text!=previous){previous=text;std::vector<unsigned char> pixels(width*height*4);auto colors=CGColorSpaceCreateDeviceRGB();auto bitmap=CGBitmapContextCreate(pixels.data(),width,height,8,width*4,colors,kCGImageAlphaPremultipliedLast);CGColorSpaceRelease(colors);CGContextSetRGBFillColor(bitmap,0,0,0,.78);CGContextFillRect(bitmap,CGRectMake(0,0,width,height));
 auto font=CTFontCreateWithName(CFSTR("Helvetica"),20,nullptr);auto white=CGColorCreateGenericRGB(1,1,1,1);const void* keys[]={kCTFontAttributeName,kCTForegroundColorAttributeName};const void* values[]={font,white};auto attrs=CFDictionaryCreate(nullptr,keys,values,2,&kCFTypeDictionaryKeyCallBacks,&kCFTypeDictionaryValueCallBacks);
 std::istringstream words(text);std::string word,line;int y=height-35;auto paint=[&](std::string l){auto string=CFStringCreateWithCString(nullptr,l.c_str(),kCFStringEncodingUTF8);auto attributed=CFAttributedStringCreate(nullptr,string,attrs);auto native=CTLineCreateWithAttributedString(attributed);CGContextSetTextPosition(bitmap,20,y);CTLineDraw(native,bitmap);y-=28;CFRelease(native);CFRelease(attributed);CFRelease(string);};
 while(words>>word){if(line.size()+word.size()>92){paint(line);line.clear();}if(!line.empty())line+=' ';line+=word;}if(!line.empty())paint(line);
 CFRelease(attrs);CFRelease(font);CGColorRelease(white);CGContextRelease(bitmap);if(!texture)glGenTextures(1,&texture);glBindTexture(GL_TEXTURE_2D,texture);glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 }
 glDisable(GL_DEPTH_TEST);glEnable(GL_TEXTURE_2D);glBindTexture(GL_TEXTURE_2D,texture);glEnable(GL_BLEND);glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);glMatrixMode(GL_PROJECTION);glPushMatrix();glLoadIdentity();glOrtho(0,viewportWidth,0,viewportHeight,-1,1);glMatrixMode(GL_MODELVIEW);glPushMatrix();glLoadIdentity();float widthOnScreen=std::min(float(width),float(viewportWidth-32)),h=widthOnScreen*height/width;float x=(viewportWidth-widthOnScreen)/2;glColor4f(1,1,1,1);glBegin(GL_QUADS);glTexCoord2f(0,0);glVertex2f(x,16);glTexCoord2f(1,0);glVertex2f(x+widthOnScreen,16);glTexCoord2f(1,1);glVertex2f(x+widthOnScreen,16+h);glTexCoord2f(0,1);glVertex2f(x,16+h);glEnd();glPopMatrix();glMatrixMode(GL_PROJECTION);glPopMatrix();glMatrixMode(GL_MODELVIEW);glDisable(GL_BLEND);
 }
};
