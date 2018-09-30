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

uniform vec4 s_ambient_color;
uniform sampler2D s_texture_0;
uniform sampler2D s_texture_1;
//uniform SAMPLER_2DMS s_texture_12;

void main() {
    //ANIMATION
    //采样两个颜色 并进行融合
    vec4 diffuse_0 = texture2D(s_texture_0,s_texcoord_0.xy);
    vec4 diffuse_1 = texture2D(s_texture_0,s_texcoord_0.zw);
    vec4 diffuse = mix(diffuse_0,diffuse_1,s_texcoord_1.z);
    //获取fade值
    diffuse *= texture2D(s_texture_1,s_texcoord_1.xy);
    //s_texcoord_2 = diffuse * diffuse_scale 漫反色颜色
    vec4 color = diffuse * s_texcoord_2;
    //乘上环境光
    color *= s_ambient_color;
    gl_FragColor = s_ambient_color;
}
