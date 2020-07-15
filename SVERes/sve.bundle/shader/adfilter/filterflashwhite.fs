#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float time;
const float PI = 3.1415926;
void main(){
    float duration = 1.0;
    
    float t_time = mod(time, duration);
    
    vec4 whiteMask = vec4(1.0, 1.0, 1.0, 1.0);
    float amplitude = abs(sin(t_time * (PI / duration)));
    
    vec4 mask = texture2D(aTexture0, v_texcoord0);
    
    gl_FragColor = mask * (1.0 - amplitude) + whiteMask * amplitude;
}
