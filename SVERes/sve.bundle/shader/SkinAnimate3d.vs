#ifdef GL_ES
precision mediump float;
#endif

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexcoord0;
attribute ivec4 aBoneID;
attribute vec4 aBoneWeight;

uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;

const int MAX_BONES = 100;
uniform mat4 uBoneMatrix[MAX_BONES];

void main(){
    gl_Position = aMatrixVP*aMatrixM*vec4(aPosition.xyz,1.0);
}
