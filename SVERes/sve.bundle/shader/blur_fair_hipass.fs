precision mediump float;
varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform sampler2D aTexture2;
uniform float blurAlpha; 
uniform vec2 tex0size;

void main(){
    vec4 mainColor = texture2D(aTexture0, v_texcoord0);
    vec4 meanColor = texture2D(aTexture1, v_texcoord0);
    vec4 blurColor = texture2D(aTexture2, v_texcoord0);
    float theta = 0.1;
    float p = clamp((min(mainColor.r, meanColor.r - 0.1) - 0.2) * 4.0, 0.0, 1.0);
    float meanVar = (blurColor.r + blurColor.g + blurColor.b) / 3.0;
    float kMin;
    vec3 resultColor;

    kMin = (1.0 - meanVar / (meanVar + theta)) * p * blurAlpha;
    float x_1 = step(0.8,kMin);
    float x_2 = step(0.5,kMin)-x_1;                      
    float x_3 = step(0.1,kMin)-x_2-x_1;                     
    float x_4 =1.0-x_1-x_2-x_3;
    kMin= kMin*(x_1+x_2*0.9 + x_3*0.5+x_4*0.08);
  
    resultColor = mix(mainColor.rgb, meanColor.rgb, kMin);
    highp float sum = 0.25*mainColor.g;
    sum += 0.125*texture2D(aTexture0,v_texcoord0+vec2(-tex0size.x,0.0)).g;
    sum += 0.125*texture2D(aTexture0,v_texcoord0+vec2(tex0size.x,0.0)).g;
    sum += 0.125*texture2D(aTexture0,v_texcoord0+vec2(0.0,-tex0size.y)).g;
    sum += 0.125*texture2D(aTexture0,v_texcoord0+vec2(0.0,tex0size.y)).g;
    sum += 0.0625*texture2D(aTexture0,v_texcoord0+vec2(tex0size.x,tex0size.y)).g;
    sum += 0.0625*texture2D(aTexture0,v_texcoord0+vec2(-tex0size.x,-tex0size.y)).g;
    sum += 0.0625*texture2D(aTexture0,v_texcoord0+vec2(-tex0size.x,tex0size.y)).g;
    sum += 0.0625*texture2D(aTexture0,v_texcoord0+vec2(tex0size.x,-tex0size.y)).g;
    float hPass = mainColor.g-sum+0.5;
    float flag = step(0.5, hPass);
    vec3 color = mix(max(vec3(0.0), (2.0*hPass + resultColor - 1.0)), min(vec3(1.0), (resultColor + 2.0*hPass - 1.0)), flag);
    color = mix(resultColor.rgb, color.rgb, 0.05);
    gl_FragColor = vec4(color, 1.0);
}