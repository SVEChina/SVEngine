#ifdef GL_ES
precision lowp float;
#endif
varying highp vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;

void main(){
    lowp vec3 iColor = texture2D(aTexture0, v_texcoord0).rgb;
    lowp vec3 meanColor = texture2D(aTexture1, v_texcoord0).rgb;
    highp vec3 diffColor = (iColor - meanColor) * 7.07;
    diffColor = min(diffColor * diffColor, 1.0);
    gl_FragColor = vec4(diffColor, 1.0);
}
