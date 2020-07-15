#ifdef GL_ES
precision highp float;
#endif
uniform sampler2D aTexture0;
const  int GAUSSIAN_SAMPLES = 9;
varying  vec2 v_texcoord0;
varying  vec2 blurCoordinates[GAUSSIAN_SAMPLES];
void main(){
vec3 sum = vec3(0.0);
vec4 fragColor=texture2D(aTexture0,v_texcoord0);
sum += texture2D(aTexture0, blurCoordinates[0]).rgb * 0.05;
sum += texture2D(aTexture0, blurCoordinates[1]).rgb * 0.09;
sum += texture2D(aTexture0, blurCoordinates[2]).rgb * 0.12;
sum += texture2D(aTexture0, blurCoordinates[3]).rgb * 0.15;
sum += texture2D(aTexture0, blurCoordinates[4]).rgb * 0.18;
sum += texture2D(aTexture0, blurCoordinates[5]).rgb * 0.15;
sum += texture2D(aTexture0, blurCoordinates[6]).rgb * 0.12;
sum += texture2D(aTexture0, blurCoordinates[7]).rgb * 0.09;
sum += texture2D(aTexture0, blurCoordinates[8]).rgb * 0.05;
gl_FragColor = vec4(sum,fragColor.a);
}
