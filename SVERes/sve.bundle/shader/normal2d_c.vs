#ifdef GL_ES
precision mediump float;
#endif

attribute vec2 aPosition;
attribute vec4 aColor;
attribute vec2 aTexcoord0;

varying vec4 v_color;
varying vec2 v_texcoord0;
uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;
uniform vec2 texcoordClip;
void main(){    
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
    v_color = aColor;
    vec4 t_pos = aMatrixVP*aMatrixM*vec4(aPosition.xy,0.0,1.0);
    gl_Position = t_pos;
}
