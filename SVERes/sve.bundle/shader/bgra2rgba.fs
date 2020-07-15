#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec2 tex0size;

void main() {
    vec4 color_bgra = texture2D(aTexture0, v_texcoord0 );
    gl_FragColor = vec4(color_bgra.b, color_bgra.g, color_bgra.r, color_bgra.a);
}
