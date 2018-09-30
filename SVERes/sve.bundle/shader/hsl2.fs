// The MIT License
// Copyright © 2014 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


// Somehow optimized HSV and HSL to RGB conversion functions.

//========================================================================

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec2 mOffset;

const float eps = 0.0000001;

vec3 hsv2rgb( in vec3 c ){
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
    return c.z * mix( vec3(1.0), rgb, c.y);
}

vec3 hsl2rgb( in vec3 c ){
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
    return c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0));
}

vec3 rgb2hsv( in vec3 c){
    vec4 k = vec4(0.0, -1.0/3.0, 2.0/3.0, -1.0);
    vec4 p = mix(vec4(c.zy, k.wz), vec4(c.yz, k.xy), (c.z<c.y) ? 1.0 : 0.0);
    vec4 q = mix(vec4(p.xyw, c.x), vec4(c.x, p.yzx), (p.x<c.x) ? 1.0 : 0.0);
    float d = q.x - min(q.w, q.y);
    return vec3(abs(q.z + (q.w - q.y) / (6.0*d+eps)), d / (q.x+eps), q.x);
}

vec3 rgb2hsl( vec3 col ){
    float minc = min( col.r, min(col.g, col.b) );
    float maxc = max( col.r, max(col.g, col.b) );
    vec3  mask = step(col.grr,col.rgb) * step(col.bbg,col.rgb);
    vec3 h = mask * (vec3(0.0,2.0,4.0) + (col.gbr-col.brg)/(maxc-minc + eps)) / 6.0;
    return vec3( fract( 1.0 + h.x + h.y + h.z ),              // H
                (maxc-minc)/(1.0-abs(minc+maxc-1.0) + eps),  // S
                (minc+maxc)*0.5 );                           // L
}
//========================================================================

void main(){
    vec4 color = texture2D(aTexture0,v_texcoord0+mOffset);
    color.xyz=rgb2hsl(color.rgb);
    color.x += _Hue; //调整偏移Hue值
    color.x = color.x%360;    //超过360的值从0开始
    color.y *= _Saturation;  //调整饱和度
    color.z *= _Value;
    color.rgb=hsl2rgb(color.xyz);
    gl_FragColor=color;
}
