#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D aTexture0;            // mean p
uniform sampler2D aTexture1;            // mean I
uniform sampler2D aTexture2;            // ak
varying vec2 v_texcoord0;
uniform int radius;                // The radius of window (in pixels).3

void main(){
    int kernelSize = radius * 2;
    vec3 pk = texture2D(aTexture0, v_texcoord0）.rgb;
    vec3 uk = texture2D(aTexture1, v_texcoord0).rgb;
    vec3 k = texture2D(aTexture2, v_texcoord0).rgb;
    vec3 bk = pk - ak * uk;
    gl_FragColor = vec4(bk, 1.0);
}
