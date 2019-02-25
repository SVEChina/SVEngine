#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
const float resolution = 1024.0;
uniform float radius;
uniform float texelWidthOffset;//若为x模糊，可传入（1.0,0.0）  y模糊  （0.0,1.0）
uniform float texelHeightOffset;
uniform float softenStrength;

highp float opacityFactor = 0.6;
lowp float factor1 = 2.782;
lowp float factor2 = 1.131;
lowp float factor3 = 1.158;
lowp float factor4 = 2.901;
lowp float factor5 = 0.979;
lowp float factor6 = 0.639;
lowp float factor7 = 0.963;

lowp vec3 rgb2hsv(lowp vec3 rgb){
    lowp vec4 a = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    highp vec4 b = mix(vec4(rgb.bg, a.wz), vec4(rgb.gb, a.xy), step(rgb.b, rgb.g));
    highp vec4 c = mix(vec4(b.xyw, rgb.r), vec4(rgb.r, b.yzx), step(b.x, rgb.r));
    highp float d = c.x - min(c.w, c.y);
    highp float e = 1.0e-10;
    lowp vec3 hsv = vec3(abs(c.z + (c.w - c.y) / (6.0 * d + e)), d / (c.x + e), c.x);
    return hsv;
}

lowp vec3 ContrastSaturationBrightness(lowp vec3 color, lowp float brt, lowp float sat, lowp float con){
    const lowp float AvgLumR = 0.5;
    const lowp float AvgLumG = 0.5;
    const lowp float AvgLumB = 0.5;
    const lowp vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);
    lowp vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
    lowp vec3 brtColor = color * brt;
    lowp vec3 intensity = vec3(dot(brtColor, LumCoeff));
    lowp vec3 satColor = mix(intensity, brtColor, sat);
    lowp vec3 conColor = mix(AvgLumin, satColor, con);
    return conColor;
}

void main (void){
    
    vec4 src=texture2D(aTexture1, v_texcoord0);
    if(radius==5.0){
        gl_FragColor = src;
        return;
    }

    lowp vec3 hsv = rgb2hsv(src.rgb);
    lowp float opacity = 1.0;
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
        gl_FragColor = src;
        return;
    }
    
    vec2 u_TextureCoordOffset[25];
    //radius可为1.4
    u_TextureCoordOffset[0]=vec2(-2. * radius,2. * radius);
    u_TextureCoordOffset[1]=vec2(-1. * radius,2. * radius);
    u_TextureCoordOffset[2]=vec2(0. * radius,2. * radius);
    u_TextureCoordOffset[3]=vec2(1. * radius,2. * radius);
    u_TextureCoordOffset[4]=vec2(2. * radius,2. * radius);
    
    u_TextureCoordOffset[5]=vec2(-2. * radius,1. * radius);
    u_TextureCoordOffset[6]=vec2(-1. * radius,1. * radius);
    u_TextureCoordOffset[7]=vec2(0. * radius,1. * radius);
    u_TextureCoordOffset[8]=vec2(1. * radius,1. * radius);
    u_TextureCoordOffset[9]=vec2(2. * radius,1. * radius);
    
    u_TextureCoordOffset[10]=vec2(-2. * radius,0. * radius);
    u_TextureCoordOffset[11]=vec2(-1. * radius,0. * radius);
    u_TextureCoordOffset[12]=vec2(0. * radius,0. * radius);
    u_TextureCoordOffset[13]=vec2(1. * radius,0. * radius);
    u_TextureCoordOffset[14]=vec2(2. * radius,0. * radius);
    
    u_TextureCoordOffset[15]=vec2(-2. * radius,-1. * radius);
    u_TextureCoordOffset[16]=vec2(-1. * radius,-1. * radius);
    u_TextureCoordOffset[17]=vec2(0. * radius,-1. * radius);
    u_TextureCoordOffset[18]=vec2(1. * radius,-1. * radius);
    u_TextureCoordOffset[19]=vec2(2. * radius,-2. * radius);
    
    u_TextureCoordOffset[20]=vec2(-2. * radius,-2. * radius);
    u_TextureCoordOffset[21]=vec2(-1. * radius,-2. * radius);
    u_TextureCoordOffset[22]=vec2(0. * radius,-2. * radius);
    u_TextureCoordOffset[23]=vec2(1. * radius,-2. * radius);
    u_TextureCoordOffset[24]=vec2(2. * radius,-2. * radius);
    
    vec4 sample[25];
    for (int i = 0; i < 25; i++){
        sample[i] = texture2D(aTexture0, v_texcoord0.xy + u_TextureCoordOffset[i]/vec2(texelWidthOffset,texelHeightOffset)+vec2(0.5/texelWidthOffset,0.5/texelHeightOffset));
    }
    
    vec4 dst = (
                (1.0  * (sample[0] + sample[4]  + sample[20] + sample[24])) +
                (4.0  * (sample[1] + sample[3]  + sample[5]  + sample[9] + sample[15] + sample[19] + sample[21] + sample[23])) +
                (7.0  * (sample[2] + sample[10] + sample[14] + sample[22])) +
                (16.0 * (sample[6] + sample[8]  + sample[16] + sample[18])) +
                (26.0 * (sample[7] + sample[11] + sample[13] + sample[17])) +
                (41.0 * sample[12])
                ) / 273.0;
    lowp vec4 blurColor  =(src*55.0+(src+dst*2.0-1.0)*45.0)/100.0;
    // lowp vec4 blurColor = texture2D(aTexture0, v_texcoord0+vec2(0.5/texelWidthOffset,0.5/texelHeightOffset));
    opacity = opacityFactor * opacity;
    lowp float mixBFactor = distance(vec3(0.0, 0.0, 0.0), max(blurColor.rgb - src.rgb, 0.0)) * factor1;
    lowp vec3 bColor = ContrastSaturationBrightness(src.rgb, factor2, 1.0, factor3);
    lowp vec3 mixBColor = mix(src.rgb, bColor.rgb, mixBFactor);
    lowp float mixDFactor = distance(vec3(0.0, 0.0, 0.0), max(src.rgb - blurColor.rgb, 0.0)) * factor4;
    lowp vec3 darkColor = ContrastSaturationBrightness(src.rgb, factor5, 1.0, factor6);
    lowp vec3 mixBDColor = mix(mixBColor.rgb, darkColor.rgb, mixDFactor);
    lowp vec3 mixBlurColor;
    
    if (factor7 < 0.999){
        lowp vec3 mixOBDColor = mix(src.rgb, mixBDColor.rgb, factor7);
        mixBlurColor = mix(mixOBDColor.rgb, blurColor.rgb, opacity);
    }
    else{
        mixBlurColor = mix(mixBDColor.rgb, blurColor.rgb, opacity);
    }
    gl_FragColor = vec4(mixBlurColor.rgb, 1.0);
}
