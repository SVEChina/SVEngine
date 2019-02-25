#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;

void main(){
    gl_FragColor = texture2D(aTexture0,v_texcoord0);
}
