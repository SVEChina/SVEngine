#ifdef GL_ES
precision mediump float;
#endif

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexcoord0;
attribute vec4 aBoneID;
attribute vec4 aBoneWeight;

uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;
const int MAX_BONES = 30;
uniform mat4 uBoneMatrix[MAX_BONES];

varying vec2 v_texcoord0;

void main(){
    
    mat4 skinMatrix =
    aBoneWeight.x * uBoneMatrix[int(aBoneID.x)] +
    aBoneWeight.y * uBoneMatrix[int(aBoneID.y)] +
    aBoneWeight.z * uBoneMatrix[int(aBoneID.z)] +
    aBoneWeight.w * uBoneMatrix[int(aBoneID.w)];
    //
    v_texcoord0 = aTexcoord0;
    gl_Position = aMatrixVP*aMatrixM*skinMatrix*vec4(aPosition,1.0);
}
