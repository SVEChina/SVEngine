#ifdef GL_ES
precision mediump float;
#endif

attribute vec2 aPosition;
attribute vec2 aColor0;
attribute vec2 aTexcoord0;

varying vec2 v_texcoord0;
uniform vec2 texcoordClip;

void main(){
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
    gl_Position = vec4(aPosition.xy,0.0,1.0);
   // gl_PointSize = 10.0;
}

