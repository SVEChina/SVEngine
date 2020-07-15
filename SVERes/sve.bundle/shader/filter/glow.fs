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

vec2 right=vec2(3.5*1.0/m_width,0.0);
vec2 top=vec2(0.0,3.5*1.0/m_height);
vec4 src  = texture2D( aTexture0, v_texcoord0);
vec3 col1 = texture2D( aTexture1, v_texcoord0).rgb;
vec3 col2 = texture2D( aTexture1, v_texcoord0+right).rgb;
vec3 col3 = texture2D( aTexture1, v_texcoord0-right).rgb;
vec3 col4 = texture2D( aTexture1, v_texcoord0+top).rgb;
vec3 col5 = texture2D( aTexture1, v_texcoord0-top).rgb;
vec3 blur = (col1+col2+col3+col4+col5)/5.0*1.25;
gl_FragColor = mix(src,vec4(blur,1.0), 0.9);

}
