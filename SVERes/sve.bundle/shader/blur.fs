#ifdef GL_ES

precision mediump float;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform float softenStrength;
varying highp vec2 v_texcoord0;
uniform vec2 tex0size;

#else

uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform  float softenStrength;
varying  vec2 v_texcoord0;
uniform vec2 tex0size;

#endif

const float opacityFactor = 0.6;
const float factor1 = 2.782;
const float factor2 = 1.131;
const float factor3 = 1.158;
const float factor4 = 2.901;
const float factor5 = 0.979;
const float factor6 = 0.639;
const float factor7 = 0.963;

vec3 rgb2hsv(vec3 rgb){
#ifdef GL_ES
    lowp vec4 a = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    highp vec4 b = mix(vec4(rgb.bg, a.wz), vec4(rgb.gb, a.xy), step(rgb.b, rgb.g));
    highp vec4 c = mix(vec4(b.xyw, rgb.r), vec4(rgb.r, b.yzx), step(b.x, rgb.r));
    highp float d = c.x - min(c.w, c.y);
    highp float e = 1.0e-10;
    lowp vec3 hsv = vec3(abs(c.z + (c.w - c.y) / (6.0 * d + e)), d / (c.x + e), c.x);
    return hsv;
#else
    vec4 a = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 b = mix(vec4(rgb.bg, a.wz), vec4(rgb.gb, a.xy), step(rgb.b, rgb.g));
    vec4 c = mix(vec4(b.xyw, rgb.r), vec4(rgb.r, b.yzx), step(b.x, rgb.r));
    float d = c.x - min(c.w, c.y);
    float e = 1.0e-10;
    vec3 hsv = vec3(abs(c.z + (c.w - c.y) / (6.0 * d + e)), d / (c.x + e), c.x);
    return hsv;
#endif
}

vec3 ContrastSaturationBrightness(vec3 color,float brt,float sat,float con){
    const vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);
    vec3 AvgLumin = vec3(0.5, 0.5, 0.5);//(AvgLumR,AvgLumG,AvgLumB)
    vec3 brtColor = color * brt;
    vec3 intensity = vec3(dot(brtColor, LumCoeff));
    vec3 satColor = mix(intensity, brtColor, sat);
    vec3 conColor = mix(AvgLumin, satColor, con);
    return conColor;
}

void main() {
    vec4 originColor = texture2D(aTexture0, v_texcoord0);
    vec3 hsv = rgb2hsv(originColor.rgb);
    float opacity = 1.0;
    if ((0.18 <= hsv.x && hsv.x <= 0.89) || hsv.z <= 0.2){
        opacity = 0.0;
    }
    if (0.16 < hsv.x && hsv.x < 0.18){
        opacity = min(opacity, (0.18 - hsv.x) / 0.02);
    }
    if (0.89 < hsv.x && hsv.x < 0.91){
        opacity = min(opacity, 1.0 - (0.91 - hsv.x) / 0.02);
    }
    if (0.2 < hsv.z && hsv.x < 0.3){
        opacity = min(opacity, 1.0 - (0.3 - hsv.z) / 0.1);
    }
    if (opacity == 0.0){
        gl_FragColor = originColor;
        return;
    }
 
    vec4 blurColor = texture2D(aTexture1, v_texcoord0);
    opacity = opacityFactor * opacity;
    float mixBFactor = distance(vec3(0.0, 0.0, 0.0), max(blurColor.rgb - originColor.rgb, 0.0)) * factor1;
    vec3 bColor = ContrastSaturationBrightness(originColor.rgb, factor2, 1.0, factor3);
    vec3 mixBColor = mix(originColor.rgb, bColor.rgb, mixBFactor);
    float mixDFactor = distance(vec3(0.0, 0.0, 0.0), max(originColor.rgb - blurColor.rgb, 0.0)) * factor4;
    vec3 darkColor = ContrastSaturationBrightness(originColor.rgb, factor5, 1.0, factor6);
    vec3 mixBDColor = mix(mixBColor.rgb, darkColor.rgb, mixDFactor);
    vec3 mixBlurColor;
    if (factor7 < 0.999){
        vec3 mixOBDColor = mix(originColor.rgb, mixBDColor.rgb, factor7);
        mixBlurColor = mix(mixOBDColor.rgb, blurColor.rgb, opacity);
    }else{
        mixBlurColor = mix(mixBDColor.rgb, blurColor.rgb, opacity);
    }
    if (softenStrength < 0.999){
        gl_FragColor = vec4(mix(originColor.rgb, mixBlurColor.rgb, softenStrength), 1.0);
    }else{
        gl_FragColor = vec4(mixBlurColor.rgb, 1.0);
    }
}
