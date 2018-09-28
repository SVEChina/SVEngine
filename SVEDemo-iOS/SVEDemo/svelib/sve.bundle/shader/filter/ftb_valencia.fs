//lvjing1

#ifdef GL_ES
precision lowp  float;
#endif


varying highp vec2 v_texcoord0;

uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform sampler2D aTexture2;

mat3 saturateMatrix = mat3(1.1402,-0.0598,-0.061,
                           -0.1174,1.0826,-0.1186,
                           -0.0228,-0.0228,1.1772);
vec3 lumaCoeffs = vec3(.3, .59, .11);
void main()
{
    vec3 texel = texture2D(aTexture0, v_texcoord0).rgb;
    texel = vec3(
    texture2D(aTexture1, vec2(texel.r, .1666666)).r,
    texture2D(aTexture1, vec2(texel.g, .5)).g,
    texture2D(aTexture1, vec2(texel.b, .8333333)).b);
    texel = saturateMatrix * texel;
    float luma = dot(lumaCoeffs, texel);
    texel = vec3(
    texture2D(aTexture2, vec2(luma, texel.r)).r,
    texture2D(aTexture2, vec2(luma, texel.g)).g,
    texture2D(aTexture2, vec2(luma, texel.b)).b);
    gl_FragColor = vec4(texel, 1.0);
}
