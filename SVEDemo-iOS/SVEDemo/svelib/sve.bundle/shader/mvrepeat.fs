#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D aTexture0;
uniform float u_mult_x;
uniform float u_mult_y;

varying vec2 v_texcoord0;

void main(){
    vec2 newTex = v_texcoord0*vec2(u_mult_x,u_mult_y);
    gl_FragColor = texture2D(aTexture0,fract(newTex));
}
