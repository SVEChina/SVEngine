#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
varying float v_depth;

void main() {
    gl_FragColor = vec4(v_depth,v_depth,v_depth,1.0);
}
