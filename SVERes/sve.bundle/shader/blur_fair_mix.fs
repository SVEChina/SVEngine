#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
const float resolution = 1024.0;
uniform float m_smooth;

//highp float opacityFactor = 0.6;
//lowp float factor1 = 2.782;
//lowp float factor2 = 1.131;
//lowp float factor3 = 1.158;
//lowp float factor4 = 2.901;
//lowp float factor5 = 0.979;
//lowp float factor6 = 0.639;
//lowp float factor7 = 0.963;

// 1.0/2.0
// 2.0/3.0
lowp vec3 rgb2hsv(lowp vec3 rgb){
    lowp vec4 a = vec4(0.0, -0.333, 0.666, -1.0);
    highp vec4 b = mix(vec4(rgb.bg, a.wz), vec4(rgb.gb, a.xy), step(rgb.b, rgb.g));
    highp vec4 c = mix(vec4(b.xyw, rgb.r), vec4(rgb.r, b.yzx), step(b.x, rgb.r));
    highp float d = c.x - min(c.w, c.y);
    highp float e = 1.0e-10;
    lowp vec3 hsv = vec3(abs(c.z + (c.w - c.y) / (6.0 * d + e)), d / (c.x + e), c.x);
    return hsv;
}
//    const lowp float AvgLumR = 0.5;
//    const lowp float AvgLumG = 0.5;
//    const lowp float AvgLumB = 0.5;
lowp vec3 ContrastSaturationBrightness(lowp vec3 color, lowp float brt, lowp float sat, lowp float con){

    const lowp vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);
    lowp vec3 AvgLumin = vec3(0.5, 0.5, 0.5);
    lowp vec3 brtColor = color * brt;
    lowp vec3 intensity = vec3(dot(brtColor, LumCoeff));
    lowp vec3 satColor = mix(intensity, brtColor, sat);
    lowp vec3 conColor = mix(AvgLumin, satColor, con);
    return conColor;
}

void main (void){
    
    vec4 src=texture2D(aTexture1, v_texcoord0);
    if(m_smooth>=5.0){
        gl_FragColor = src;
        return;
    }
    
  //  lowp vec3 hsv = rgb2hsv(src.rgb);
    lowp float opacity = 1.0;
//    if ((0.18 <= hsv.x && hsv.x <= 0.89) || hsv.z <= 0.2){
//        opacity = 0.0;
//    }
//    if (0.16 < hsv.x && hsv.x < 0.18){
//        opacity = min(opacity, (0.18 - hsv.x) / 0.02);
//    }
//
//    if (0.89 < hsv.x && hsv.x < 0.91){
//        opacity = min(opacity, 1.0 - (0.91 - hsv.x) / 0.02);
//    }
//
//    if (0.2 < hsv.z && hsv.x < 0.3){
//        opacity = min(opacity, 1.0 - (0.3 - hsv.z) / 0.1);
//    }
//
//    if (opacity == 0.0){
//        opacity=0.5;
//    }
    vec4 dst =texture2D(aTexture0, v_texcoord0);
    lowp vec4 blurColor  =(src*40.0+(src+dst*2.0-1.0)*60.0)/100.0;
    blurColor = max(vec4(0.0), blurColor);
    blurColor = min(vec4(1.0), blurColor);
    opacity = 0.6 * opacity;
    lowp float mixBFactor = distance(vec3(0.0, 0.0, 0.0), max(blurColor.rgb - src.rgb, 0.0)) * 2.782;
    lowp vec3 bColor = ContrastSaturationBrightness(src.rgb, 1.131, 1.0, 1.158);
    lowp vec3 mixBColor = mix(src.rgb, bColor.rgb, mixBFactor);
    lowp float mixDFactor = distance(vec3(0.0, 0.0, 0.0), max(src.rgb - blurColor.rgb, 0.0)) * 2.901;
    lowp vec3 darkColor = ContrastSaturationBrightness(src.rgb, 0.979, 1.0, 0.639);
    lowp vec3 mixBDColor = mix(mixBColor.rgb, darkColor.rgb, mixDFactor);
    lowp vec3 mixBlurColor;
    lowp vec3 mixOBDColor = mix(src.rgb, mixBDColor.rgb, 0.963);
    mixBlurColor = mix(mixOBDColor.rgb, blurColor.rgb, opacity);
    gl_FragColor = vec4(mixBlurColor.rgb, 1.0);
}
