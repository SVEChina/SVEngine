#include "SVColor.h"

using namespace sv;

//ARGB 格式
SVColor::SVColor()
: r(1.0f)
, g(1.0f)
, b(1.0f)
, a(1.0f) {
}

SVColor::SVColor(f32 r, f32 g, f32 b, f32 a){
    setColor(r, g, b, a);
}

void SVColor::operator=(SVColor& _color) {
    r = _color.r;
    g = _color.g;
    b = _color.b;
    a = _color.a;
}

void SVColor::setColor(f32 _r, f32 _g, f32 _b, f32 _a) {
    r = _r;
    g = _g;
    b = _b;
    a = _a;
}

void SVColor::setColorARGB(u32 color){
    b = (color&0x000000ff)/255.0f;          //
    g = ((color&0x0000ff00)>>8)/255.0f;     //
    r = ((color&0x00ff0000)>>16)/255.0f;    //
    a = ((color&0xff000000)>>24)/255.0f;    //
}

u32 SVColor::getColorARGB(){
    u32 t_color = 0;
    t_color = (u32)(a*255.0f);
    t_color = (t_color<<8)|((u32)(r*255.0f));
    t_color = (t_color<<8)|((u32)(g*255.0f));
    t_color = (t_color<<8)|((u32)(b*255.0f));
    return t_color;
}

u32 SVColor::transRgbaToID(u8 r,u8 g,u8 b,u8 a){
    //b,g,r,a
    u32 t_color = 0;
    t_color = (t_color|a)<<8;
    t_color = (t_color|r)<<8;
    t_color = (t_color|g)<<8;
    t_color = (t_color|b);
    return t_color;
}
