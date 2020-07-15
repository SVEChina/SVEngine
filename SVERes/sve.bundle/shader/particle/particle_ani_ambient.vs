#ifdef GL_ES
precision highp float;
#endif

attribute vec3 s_attribute_0;
attribute vec4 s_attribute_1; //xy表示纹理坐标(255,255) zw表示颜色(255,255) 需要在外面归一化传入
attribute vec4 s_attribute_2;

varying vec4 s_texcoord_0;
varying vec4 s_texcoord_1;
varying vec4 s_texcoord_2;
varying vec4 s_texcoord_3;

uniform mat4 aMatrixP;
uniform mat4 s_p_transform;
uniform float s_particles_radius;
uniform float s_particles_fade;

uniform vec4 animation_transform;
uniform vec4 diffuse_color;
uniform float diffuse_scale;
void main()
{
    //计算顶点
    vec4 vertex = s_p_transform*vec4(s_attribute_0.xyz,1.0);
    //纹理坐标(0，255的值)
    vec4 texcoord = s_attribute_1.xyxy;
    //计算动画
    //animation(x,y)表示对fade的采样坐标 (z)表示对两个颜色值的融合值
    vec3 animation = vec3(s_attribute_1.w + s_attribute_1.z * (1.0 / 256.0),animation_transform.w,0.0);
    //ANIMATION begin
    vec2 isize = 1.0 / animation_transform.xy;
    float size = animation_transform.x * animation_transform.y;
    //取小数部分
    float frame = fract(animation.x * animation_transform.z) * size;
    //小于目标的整数部分
    float frame_0 = floor(frame);
    float frame_1 = mod(frame_0 + 1.0,size);
    //计算两个纹理坐标
    texcoord.x += mod(frame_0,animation_transform.x);
    texcoord.z += mod(frame_1,animation_transform.x);
    texcoord.y += floor(frame_0 * isize.x);
    texcoord.w += floor(frame_1 * isize.x);
    animation.z = frame - frame_0;
    texcoord *= isize.xyxy;
    //ANIMATION end
    //计算alpha褪色
    vec4 color = vec4(diffuse_color.xyz * diffuse_scale,diffuse_color.w * s_particles_fade);
//#ifdef ALPHA_FADE
//    vec4 color = vec4(diffuse_color.xyz * diffuse_scale,diffuse_color.w * s_particles_fade);
//#else
//    vec4 color = vec4(diffuse_color.xyz * diffuse_scale,diffuse_color.w) * s_particles_fade;
//#endif
    //位置
    gl_Position = aMatrixP*vertex;
    //纹理坐标
    s_texcoord_0 = texcoord;
    //动画
    s_texcoord_1.xyz = animation;
    //
    s_texcoord_2 = color;
    //颜色
    s_texcoord_3 = s_attribute_2;
}
