#ifdef GL_ES
precision mediump float;
#endif

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexcoord0;

uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;

varying vec2 v_texcoord0;

void main(){    
    v_texcoord0 = aTexcoord0;
    vec4 t_pos = aMatrixVP*aMatrixM*vec4(aPosition,1.0);
    gl_Position = t_pos;
}
