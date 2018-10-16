#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec2 tex0size;

void main() {
    gl_FragColor = texture2D(aTexture0, v_texcoord0 );
}
