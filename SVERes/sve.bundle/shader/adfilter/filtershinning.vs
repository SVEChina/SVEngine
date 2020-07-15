#ifdef GL_ES
precision mediump float;
#endif

attribute vec2 aPosition;
attribute vec2 aTexcoord0;

varying vec2 v_texcoord0;
varying float v_lerp;
uniform vec2 texcoordClip;
uniform float lerp;
void main(){
    float scale = lerp*0.3 + 1.0;
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
    gl_Position = vec4(aPosition.x*scale, aPosition.y*scale, 0.0, 1.0);
    v_lerp = lerp;
}

