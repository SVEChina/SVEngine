#ifdef GL_ES
precision mediump float;
#endif

attribute vec2 aPosition;
attribute vec2 aTexcoord0;

uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;
uniform vec2 texcoordClip;

varying vec2 v_texcoord0;

void main(){
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
    gl_Position = aMatrixVP*aMatrixM*vec4(aPosition.xy,0.0,1.0);
}
