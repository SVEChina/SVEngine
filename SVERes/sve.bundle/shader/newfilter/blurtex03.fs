

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float m_smooth;
uniform float m_width;
uniform float m_height;


void main(){
vec2 uv = v_texcoord0;
vec4 sum = vec4(0);
// Vertical Blur
vec4 accumulation = vec4(0);
vec4 weightsum = vec4(0);
float kernel = 10.0;
float weight = 1.0;
for (float i = -kernel; i <= kernel; i++){
accumulation += texture2D(aTexture0, uv + vec2(i * m_width, i * m_height));
weightsum += weight;
}
sum = accumulation / weightsum;
gl_FragColor = sum;
}
