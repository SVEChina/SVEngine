#ifdef GL_ES
precision highp float;
#endif

attribute vec2 aPosition;
attribute vec2 aTexcoord0;

uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;
uniform vec2 u_grid_size;

varying vec2 v_texcoord0;
varying vec2 v_pos;

void main(){
    v_pos = aPosition;
    v_texcoord0 = aTexcoord0*u_grid_size;
    vec4 t_pos = aMatrixVP*aMatrixM*vec4(aPosition,0.0,1.0);
    gl_Position = t_pos;
}
