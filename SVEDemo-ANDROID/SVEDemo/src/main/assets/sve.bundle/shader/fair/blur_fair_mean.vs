precision highp float;

attribute vec2 aPosition;
attribute vec2 aColor0;
attribute vec2 aTexcoord0;
uniform vec2 texcoordClip;
varying vec2 v_texcoord0;

void main(void){
    gl_Position = vec4(aPosition,0.0,1.0);
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
}
