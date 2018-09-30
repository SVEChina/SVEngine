#ifdef GL_ES
precision highp float;
#endif

attribute vec3 aPosition;
attribute vec2 aTexcoord0;

varying vec2 v_texcoord0;

uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;

void main()
{
     gl_Position  = aMatrixVP * aMatrixM * vec4(aPosition , 1.0);
     v_texcoord0    = aTexcoord0;
}
