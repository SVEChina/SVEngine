//lvjing2
#ifdef GL_ES
precision lowp  float;
#endif

varying highp vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
void main()
{
    vec3 texel = texture2D(aTexture0, v_texcoord0).rgb;
    texel = vec3(
              texture2D(aTexture1, vec2(texel.r, .16666)).r,
              texture2D(aTexture1, vec2(texel.g, .5)).g,
              texture2D(aTexture1, vec2(texel.b, .83333)).b);
    gl_FragColor = vec4(texel, 1.0);
}
