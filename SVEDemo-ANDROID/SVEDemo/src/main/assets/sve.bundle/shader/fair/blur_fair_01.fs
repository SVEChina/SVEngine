#ifdef GL_ES
precision lowp float;
#endif
uniform sampler2D aTexture0;
varying highp vec2 v_texcoord0;
varying highp vec4 textureShift_1;
varying highp vec4 textureShift_2;
varying highp vec4 textureShift_3;
varying highp vec4 textureShift_4;

void main(){
    mediump vec3 sum = texture2D(aTexture0, v_texcoord0).rgb;
    sum += texture2D(aTexture0, textureShift_1.xy).rgb;
    sum += texture2D(aTexture0, textureShift_1.zw).rgb;
    sum += texture2D(aTexture0, textureShift_2.xy).rgb;
    sum += texture2D(aTexture0, textureShift_2.zw).rgb;
    sum += texture2D(aTexture0, textureShift_3.xy).rgb;
    sum += texture2D(aTexture0, textureShift_3.zw).rgb;
    sum += texture2D(aTexture0, textureShift_4.xy).rgb;
    sum += texture2D(aTexture0, textureShift_4.zw).rgb;
    gl_FragColor = vec4(sum * 0.1111, 1.0);
}
