#ifdef GL_ES
precision highp float;
#endif
/* s_texture_0 is diffuse texture   漫反射纹理
 * s_texture_1 is attenuation texture 衰减纹理
 * s_texture_12 is deferred depth texture
 */
varying vec4 s_texcoord_0;
varying vec4 s_texcoord_1;
varying vec4 s_texcoord_2;
varying vec4 s_texcoord_3;

uniform vec4 s_ambient_color;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;

void main() {
    //ANIMATION
    //采样两个颜色 并进行融合
    vec4 diffuse_0 = texture2D(aTexture0,s_texcoord_0.xy);
    vec4 diffuse_1 = texture2D(aTexture0,s_texcoord_0.zw);
    vec4 diffuse = mix(diffuse_0,diffuse_1,s_texcoord_1.z);
    //获取fade值
    vec4 fadeColor_0 = texture2D(aTexture1,s_texcoord_0.xy);
    vec4 fadeColor_1 = texture2D(aTexture1,s_texcoord_0.zw);
    vec4 fadeColor = mix(fadeColor_0,fadeColor_1,s_texcoord_1.z);
    //
    vec4 color = diffuse * fadeColor * s_texcoord_2;
    //颜色
    color *= s_texcoord_3;
    //乘上环境光
    color *= s_ambient_color;
    gl_FragColor = color;
}
