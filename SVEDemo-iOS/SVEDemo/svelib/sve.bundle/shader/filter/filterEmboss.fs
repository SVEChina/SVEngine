#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D aTexture0;
varying vec2 v_texcoord0;
uniform vec2 samplerSteps;
uniform float stride;
uniform float intensity;
uniform vec2 norm;

void main() {
    vec4 src = texture2D(aTexture0, v_texcoord0);
    vec3 tmp = texture2D(aTexture0, v_texcoord0 + samplerSteps * stride * norm).rgb - src.rgb + 0.5;
    float f = (tmp.r + tmp.g + tmp.b) / 3.0;
    gl_FragColor = vec4(mix(src.rgb, vec3(f, f, f), intensity), src.a);
}
