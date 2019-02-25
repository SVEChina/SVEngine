#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float m_gamma;

void main(){
    vec4 color = texture2D(aTexture0, v_texcoord0);
    gl_FragColor = vec4(pow(color.rgb, vec3(m_gamma)), color.w);
    
}
