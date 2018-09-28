#ifdef GL_ES
precision mediump float;
#endif

attribute vec2 aPosition;
attribute vec4 aColor;
attribute vec2 aTexcoord0;

varying vec4 v_color;
varying vec2 v_texcoord0;
varying vec2 v_texcoord1;
uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;
uniform vec2 tex1size;
void main(){    
    v_texcoord0 = aTexcoord0;
//    v_texcoord1 = (aPosition.xy*tex1size - 0.5) + 0.5;
    v_color = aColor/255.0;
    vec4 t_pos = aMatrixVP*aMatrixM*vec4(aPosition.xy,0.0,1.0);
//    v_texcoord1 = t_pos.xy*0.5 + 0.5;
    gl_Position = t_pos;
}
