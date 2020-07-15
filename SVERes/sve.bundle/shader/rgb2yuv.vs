#ifdef GL_ES
precision mediump float;
#endif

attribute vec2 aPosition;
attribute vec2 aTexcoord0;

varying vec2 v_texcoord0;

void main(){
    v_texcoord0 = aTexcoord0;
    gl_Position = vec4(aPosition.xy,0.0,1.0);
    gl_PointSize = 3.0;
}
