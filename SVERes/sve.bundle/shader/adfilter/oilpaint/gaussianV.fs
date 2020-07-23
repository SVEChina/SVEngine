#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec2 tex0size;
const int windowSize = 4;
const float sigma = 0.5;
void main(){
    vec2 uv = v_texcoord0;
    float bound_y = 1.0/tex0size.y;
    vec4 sum =vec4(0.0, 0.0, 0.0, 0.0);
    float weightSum = 0.0;
    for(int i=-windowSize;i<=windowSize;i++){
        float t_i = float(i);
        float weight = exp(-sigma*t_i*t_i);
        vec4 color = texture2D(aTexture0, vec2(uv.x, clamp(uv.y+t_i*bound_y, 0.0, 1.0)))*weight;
        sum += color;
        weightSum += weight;
    }
    gl_FragColor = sum/weightSum;
}
