#ifdef GL_ES
precision mediump float;
#endif

attribute vec3 aPosition;
attribute vec4 aColor;
attribute vec3 aInsOffset;//多实例偏移
attribute vec2 aTexcoord0;
uniform vec2 texcoordClip;
uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;
varying vec4 v_color;
varying vec2 v_texcoord0;
uniform vec3 u_up;
uniform vec3 u_viewPos;
void main(){
    v_color = aColor;
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
    mat4 nMatrixM = aMatrixM;
//    nMatrixM[3] = vec4(aInsOffset, 0.0f);
    gl_Position = aMatrixVP * nMatrixM * vec4( aPosition.xyz + aInsOffset, 1.0 );
    gl_PointSize = 10.0;
}

