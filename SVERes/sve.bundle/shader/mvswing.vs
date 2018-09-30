#ifdef GL_ES
precision mediump float;
#endif

attribute vec2 aPosition;
attribute vec2 aTexcoord0;

uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;
uniform float u_offx;
uniform float u_offy;

varying vec2 v_texcoord0;

void main(){
    v_texcoord0 = aTexcoord0;
    vec4 srcpos = vec4(aPosition.x + u_offx,aPosition.y+u_offy,0.0,1.0);
    gl_Position = aMatrixVP*aMatrixM*srcpos;
}
