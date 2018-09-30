#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D aTexture0;            // p
uniform sampler2D aTexture1;            // I
uniform sampler2D aTexture2;            // mean p
uniform sampler2D aTexture3;            // mean I
uniform int     radius;                // The radius of window (in pixels). 3
uniform float   eps;            // The regularization parameter. 0.00001
varying vec2 v_texcoord0;

void main(){
    int kernelSize = radius * 2;
    vec3 uk_color = texture2D(aTexture3, v_texcoord0).rgb;
    float uk_gray = dot(uk_color.rgb, vec3(0.299, 0.587, 0.114));
    // Computing ak
    vec3 sum = vec3(0.0);
    float sum_I2 = 0.0;
    for (int i = -kernelSize; i <= kernelSize; i++){
        for (int j = -kernelSize; j <= kernelSize; j++){
            // for numerator
            vec3 P = texture2D(aTexture0, v_texcoord0 + ivec2(i, j)).rgb;
            vec3 I = texture2D(aTexture1, v_texcoord0 + ivec2(i, j)).rgb;
            vec3 pk = texture2D(aTexture2, v_texcoord0 + ivec2(i, j)).rgb;
            vec3 uk = texture2D(aTexture3, v_texcoord0 + ivec2(i, j)).rgb;
            sum += P * I - uk * pk;
            // for denominator
            float I_gray = dot(I.rgb, vec3(0.299, 0.587, 0.114));
            sum_I2 += pow(I_gray - uk_gray, 2.0);
        }
    }
    float size = float((2 * kernelSize + 1) * (2 * kernelSize + 1));
    float var = sum_I2 / (size - 1.0f);            // the variance of I in wk in uk
    vec3 ak_numerator = sum / size;    // normalizing
    float ak_denominator = var - eps;
    vec3 ak = ak_numerator / ak_denominator;
    gl_FragColor = vec4(ak, 1.0);
}
