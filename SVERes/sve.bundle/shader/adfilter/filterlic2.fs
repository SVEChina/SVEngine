#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform sampler2D aTexture2;
uniform vec2 tex0size;
uniform vec2 tex1size;
uniform vec2 tex2size;
uniform float time;
const int uLength = 5;

void main(){
    vec2 uv = v_texcoord0;
    vec3 col = texture2D(aTexture0, uv).rgb;
    vec2 v = (texture2D(aTexture1, uv).xy - 0.5) * 2.0;
//    vec2 velocity = vec2(cos((uv.x - 2.0*time)), sin((uv.x - 2.0*time)));
//    v *= velocity;
    v *= 1./tex2size;
    int w = 0;
    vec2 st0 = uv;
    for(int i = 0; i < uLength; i++) {
        st0 += v;
        vec3 n = texture2D(aTexture0, st0).rgb;
        col += n;
        w++;
    }

    vec2 st1 = uv;
    for(int i = 0; i < uLength; i++) {
        st1 -= v;
        vec3 n = texture2D(aTexture0, st1).rgb;
        col += n;
        w++;
    }
    col /= float(w);
    gl_FragColor = vec4( col, 1. );
}
