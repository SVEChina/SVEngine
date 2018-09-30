#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D aTexture0;            // I
uniform sampler2D aTexture1;            // a
uniform sampler2D aTexture2;            // b
varying vec2 v_texcoord0;
uniform int radius = 3;                // The radius of window (in pixels).

void main(){
    int kernelSize = radius * 2;
    vec3 I = texture2D(aTexture0, v_texcoord0).rgb;
    // Computing mean
    vec3 sum_a = vec3(0.0);
    vec3 sum_b = vec3(0.0);
    for (int i = -kernelSize; i <= kernelSize; i++){
        for (int j = -kernelSize; j <= kernelSize; j++){
            sum_a += texture2D(aTexture1, v_texcoord0 + ivec2(i, j)).rgb;
            sum_b += texture2D(aTexture2, v_texcoord0 + ivec2(i, j)).rgb;
        }
    }
    vec3 mean_a = sum_a / float((2 * kernelSize + 1) * (2 * kernelSize + 1));    // normalizing
    vec3 mean_b = sum_b / float((2 * kernelSize + 1) * (2 * kernelSize + 1));    // normalizing
    vec3 q = mean_a * I + mean_b;
    gl_FragColor = vec4(q, 1.0);
}
