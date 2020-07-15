#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform float m_smooth;
uniform float m_width;
uniform float m_height;

void main(){
    vec4 overlay  = texture2D( aTexture1, v_texcoord0).rgba;
    vec4  base= texture2D( aTexture0, v_texcoord0).rgba;
//    float k = tan( (45.0 + 44.0*0.0) / 180.0 * 3.1415926 );
//    base.rgb=(base.rgb - 0.5 * (1.0 - 0.5)) * k + 0.5 * (1.0 + 0.5);
//  vec4 rgba=vec4(base.rgb+overlay.rgb-base.rgb*overlay.rgb,base.a);
    gl_FragColor=base;
}
