#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float intensity;
void main()
{
    vec4 src = texture2D(aTexture0, v_texcoord0);
    float lum = (max(max(src.r, src.g),src.b) + min(min(src.r, src.g), src.b)) / 2.0;
    gl_FragColor = vec4(mix(vec3(lum), src.rgb, intensity), src.a);
}
