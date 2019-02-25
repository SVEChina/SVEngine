#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D           aTexture0;
varying vec2                v_texcoord0;

void main() {
    vec2 uv = v_texcoord0;
    vec3 tex = texture2D( aTexture0, uv ).rgb;
    float shade = dot(tex, vec3(0.333333));
    vec3 col = mix(vec3(0.1, 0.36, 0.8) * (1.0-2.0*abs(shade-0.5)), vec3(1.06, 0.8, 0.55), 1.0-shade);
    gl_FragColor = vec4(col,1.0);
}
