precision highp float;

uniform float               iGlobalTime;
uniform sampler2D           aTexture0;
varying vec2                v_texcoord0;

void main() {
    float stongth = 0.3;
    vec2 uv = v_texcoord0;
    float waveu = sin((uv.y + iGlobalTime) * 20.0) * 0.5 * 0.05 * stongth;
    gl_FragColor = texture2D(aTexture0, uv + vec2(waveu, 0));
}
