#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D aTexture0;
uniform int     radius ;                // The radius of window (in pixels).3
varying vec2 v_texcoord0;
void main(){
    int kernelSize = radius * 2;
    // Computing mean
    vec3 sum_p = vec3(0.0);
    for (int i = -kernelSize; i <= kernelSize; i++)
    for (int j = -kernelSize; j <= kernelSize; j++)
    sum_p += texture2D(aTexture0, v_texcoord0 + ivec2(i, j)).rgb;
    vec3 mean = sum_p / float((2 * kernelSize + 1) * (2 * kernelSize + 1));    // normalizing
    gl_FragColor = vec4(mean, 1.0);
}
