#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float m_exposure;

void main(){
    vec4 color = texture2D(aTexture0, v_texcoord0);
    gl_FragColor = vec4(color.rgb * pow(2.0, m_exposure), color.w);
}
