#ifdef GL_ES
precision highp float;
#endif

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexcoord0;

uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;
uniform vec2 texcoordClip;
uniform mat4 aMatrixVPlight;

varying vec2 v_texcoord0;

varying float v_depth;

varying vec4 v_posLightSpace;

void main(){
    v_texcoord0 = aTexcoord0;
    gl_Position = aMatrixVP*aMatrixM*vec4(aPosition.xyz,1.0);
    v_depth=gl_Position.z/gl_Position.w;
    v_posLightSpace=aMatrixVPlight*aMatrixM*vec4(aPosition.xyz,1.0);
}

