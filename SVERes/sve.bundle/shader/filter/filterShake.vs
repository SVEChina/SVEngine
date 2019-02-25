#ifdef GL_ES
precision mediump float;
#endif

attribute vec2 aPosition;
attribute vec2 aTexcoord0;

varying vec2 v_texcoord0;
//uniform float offsetScale;
uniform float offsetY;

void main()
{
    v_texcoord0 = aTexcoord0;
//    vec4 t_pos = vec4(aPosition.x*offsetScale,aPosition.y*offsetScale,0.0,1.0);
    vec4 t_pos = vec4(aPosition.x,(aPosition.y + offsetY),0.0,1.0);
    gl_Position = t_pos;
}
