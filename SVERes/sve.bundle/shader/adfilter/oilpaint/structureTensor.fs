#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec2 tex0size;

vec2 computeUV(vec2 uv, vec2 offset){
    vec2 bound = 1.0/tex0size;
    vec2 n_uv = vec2(clamp(uv.x+offset.x*bound.x, 0.0, 1.0), clamp(uv.y+offset.y*bound.y, 0.0, 1.0));
    return n_uv;
}

void main(){
    vec2 uv = v_texcoord0;
    // rotational symmetric derivative filter
    float s_1_1 = texture2D(aTexture0, computeUV(uv, vec2(-1.0, -1.0))).r;
    float s0_1 = texture2D(aTexture0, computeUV(uv, vec2(0.0, -1.0))).r;
    float s1_1 = texture2D(aTexture0, computeUV(uv, vec2(1.0, -1.0))).r;
    float s_10 = texture2D(aTexture0, computeUV(uv, vec2(-1.0, 0.0))).r;
    float s10 = texture2D(aTexture0, computeUV(uv, vec2(1.0, 0.0))).r;
    float s_11 = texture2D(aTexture0, computeUV(uv, vec2(-1.0, 1.0))).r;
    float s01 = texture2D(aTexture0, computeUV(uv, vec2(0.0, 1.0))).r;
    float s11 = texture2D(aTexture0, computeUV(uv, vec2(1.0, 1.0))).r;

    float dx = (-1.0*s_11 + 1.0*s11 - 2.0*s_10 + 2.0*s10 - 1.0*s_1_1 + 1.0*s1_1);
    float dy = -(-1.0*s_11 + 1.0*s_1_1 - 2.0*s01 + 2.0*s0_1 - 1.0*s11 + 1.0*s1_1);

    //float dx = (p*s_11-p*s11+(1-2*p)*s_10+(2*p-1)*s10+p*s_1_1-p*s1_1)*0.5;
    //float dy = -(p*s_11-p*s_1_1+(1-2*p)*s01+(2*p-1)*s0_1+p*s11-p*s1_1)*0.5;
    // eigen
    vec4 tensor = vec4(dx*dx,dx*dy,dy*dy,0.0);

    gl_FragColor  = tensor;
}
