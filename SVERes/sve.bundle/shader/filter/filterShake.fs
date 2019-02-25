#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D aTexture0;
varying vec2 v_texcoord0;

void main()
{
    gl_FragColor = texture2D(aTexture0,v_texcoord0);
}

