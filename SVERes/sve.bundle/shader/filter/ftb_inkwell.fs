#ifdef GL_ES
precision lowp  float;
#endif

varying highp vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform int disableEffect;
void main()
{
    vec3 texel = texture2D(aTexture0, v_texcoord0).rgb;
    texel = vec3(dot(vec3(0.3, 0.6, 0.1), texel));
    texel = vec3(texture2D(aTexture1, vec2(texel.r, .16666)).r);
    gl_FragColor = vec4(texel, 1.0);
}
