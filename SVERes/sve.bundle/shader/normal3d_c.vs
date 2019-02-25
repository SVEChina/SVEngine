attribute vec3 aPosition;
attribute vec4 aColor;
attribute vec3 aNormal;
attribute vec2 aTexcoord0;

varying vec3 v_Normal;
varying vec4 v_color;
varying vec2 v_texcoord0;
varying vec3 v_FragPos;
uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;
void main(){
    v_color = aColor;
    v_texcoord0 = aTexcoord0;
    gl_Position = aMatrixVP*aMatrixM*vec4(aPosition.xyz,1.0);
    v_FragPos = vec3(aMatrixM * vec4(aPosition.xyz, 1.0));
    v_Normal = aNormal;
}
