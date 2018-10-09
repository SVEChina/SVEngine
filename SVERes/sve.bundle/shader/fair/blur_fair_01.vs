#ifdef GL_ES
precision highp float;
#endif
attribute vec2 aPosition;
attribute vec2 aColor0;
attribute vec2 aTexcoord0;
uniform vec2 texcoordClip;
varying vec2 v_texcoord0;
uniform vec2 tex0size;
varying vec4 textureShift_1;
varying vec4 textureShift_2;
varying vec4 textureShift_3;
varying vec4 textureShift_4;

void main(void){
    gl_Position = vec4(aPosition,0.0,1.0);
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
    textureShift_1 = vec4(v_texcoord0 - tex0size, v_texcoord0.st + tex0size);
    textureShift_2 = vec4(v_texcoord0 - 2.0 * tex0size, v_texcoord0 + 2.0 * tex0size);
    textureShift_3 = vec4(v_texcoord0 - 3.0 * tex0size, v_texcoord0 + 3.0 * tex0size);
    textureShift_4 = vec4(v_texcoord0 - 4.0 * tex0size, v_texcoord0 + 4.0 * tex0size);
}
