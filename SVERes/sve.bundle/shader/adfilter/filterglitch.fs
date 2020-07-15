#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
//這是個二階向量，x是橫向偏移的值，y是閾值
uniform vec2 uScanLineJitter;
//顏色偏移的值
uniform float uColorDrift;
float nrand(in float x, in float y){
    return fract(sin(dot(vec2(x, y), vec2(12.9898, 78.233))) * 43758.5453);
}
void main(){
    float u = v_texcoord0.x;
    float v = v_texcoord0.y;
    float jitter = nrand(v,0.0) * 2.0 - 1.0;
    float drift = uColorDrift;
    float offsetParam = step(uScanLineJitter.y,abs(jitter));
    jitter = jitter * offsetParam * uScanLineJitter.x;
    vec4 color1 = texture2D(aTexture0,fract(vec2( u + jitter,v)));
    vec4 color2 = texture2D(aTexture0,fract(vec2(u + jitter + v*drift ,v)));
    gl_FragColor = vec4(color1.r,color2.g,color1.b,1.0);
}
