#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec2 tex0size;

void main(){
    float scaleFactor = 0.6;
    vec2 uv = v_texcoord0;
    vec3 kernel_0 = vec3(2., 0., 2.);
    vec3 kernel_1 = vec3(0., 0., 0.);
    vec3 kernel_2 = vec3(3., 1., -6.);
    vec4 o = vec4(1., 1., 1., 1.);
    mat3 kernel = mat3(kernel_0.xyz, kernel_1.xyz, kernel_2.xyz);
    float offset = 0.005;
    //卷积计算
    vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
    sum += texture2D(aTexture0, uv + vec2(-offset, -offset)) * kernel[0][0];
    sum += texture2D(aTexture0, uv + vec2(0.0, -offset)) * kernel[0][1];
    sum += texture2D(aTexture0, uv + vec2(offset, -offset)) * kernel[0][2];
    sum += texture2D(aTexture0, uv + vec2(-offset, 0.0)) * kernel[1][0];
    sum += texture2D(aTexture0, uv + vec2(0.0, 0.0)) * kernel[1][1];
    sum += texture2D(aTexture0, uv + vec2(offset, 0.0)) * kernel[1][2];
    sum += texture2D(aTexture0, uv + vec2(-offset, offset)) * kernel[2][0];
    sum += texture2D(aTexture0, uv + vec2(0.0, offset)) * kernel[2][1];
    sum += texture2D(aTexture0, uv + vec2(offset, offset)) * kernel[2][2];
    //加权因子(调整亮度)
    sum *= scaleFactor;
    o *= sum;

    gl_FragColor = o;
    
    float pixelGrey = dot(o.rgb, vec3(0.3, 0.59, 0.11));
    vec3 pixelColor = vec3(pixelGrey);
    gl_FragColor = vec4(pixelColor, 1.0);
}
