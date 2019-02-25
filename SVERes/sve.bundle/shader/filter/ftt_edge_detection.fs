#ifdef GL_ES
#extension GL_OES_standard_derivatives : enable
precision mediump float;
#endif

uniform sampler2D           aTexture0;
varying vec2                v_texcoord0;

void main() {
    vec4 color =  texture2D(aTexture0, v_texcoord0);
    float gray = length(color.rgb);
    gl_FragColor = vec4(vec3(step(0.06, length(vec2(dFdx(gray), dFdy(gray))))), 1.0);
}
