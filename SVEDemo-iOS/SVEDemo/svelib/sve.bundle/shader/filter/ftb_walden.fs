#ifdef GL_ES
precision lowp  float;
#endif

varying highp vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1; //map
uniform sampler2D aTexture2;
void main()
{
    vec3 texel = texture2D(aTexture0, v_texcoord0).rgb;
    texel = vec3(
              texture2D(aTexture1, vec2(texel.r, .16666)).r,
              texture2D(aTexture1, vec2(texel.g, .5)).g,
              texture2D(aTexture1, vec2(texel.b, .83333)).b);
    vec2 tc = (2.0 * v_texcoord0) - 1.0;
    float d = dot(tc, tc);
    vec2 lookup = vec2(d, texel.r);
    texel.r = texture2D(aTexture2, lookup).r;
    lookup.y = texel.g;
    texel.g = texture2D(aTexture2, lookup).g;
    lookup.y = texel.b;
    texel.b= texture2D(aTexture2, lookup).b;
    gl_FragColor = vec4(texel, 1.0);
}
