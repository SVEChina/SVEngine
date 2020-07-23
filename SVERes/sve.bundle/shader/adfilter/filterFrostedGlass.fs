#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec2 tex0size;

float rand(vec2 uv) {
 
    float a = dot(uv, vec2(92., 80.));
    float b = dot(uv, vec2(41., 62.));
    
    float x = sin(a) + cos(b) * 51.;
    return fract(x);
    
}

void main(){
    vec2 uv = v_texcoord0;
    vec2 rnd = vec2(rand(uv), rand(uv));
    
    uv += rnd * .05;
    gl_FragColor = texture2D(aTexture0, uv);
}
