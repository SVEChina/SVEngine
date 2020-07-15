#ifdef GL_ES
precision mediump float;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
varying highp vec2 v_texcoord0;
uniform vec2 tex0size;
uniform lowp float shadows;
uniform lowp float highlights;
uniform lowp float contrast;
uniform lowp float saturation;
uniform lowp float brightness;
uniform lowp float whitening;
uniform highp float gamma;
uniform lowp float redShift;
uniform lowp float greenShift;
uniform lowp float blueShift;
uniform lowp float sdredShift;
uniform lowp float sdgreenShift;
uniform lowp float sdblueShift;
uniform lowp float hhredShift;
uniform lowp float hhgreenShift;
uniform lowp float hhblueShift;
uniform highp float temperature;
uniform highp float tint;

#else

uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
varying vec2 v_texcoord0;
uniform vec2 tex0size;
uniform float shadows;
uniform float highlights;
uniform float contrast;
uniform float saturation;
uniform float brightness;
uniform float whitening;
uniform float gamma;
uniform float redShift;
uniform float greenShift;
uniform float blueShift;
uniform float sdredShift;
uniform float sdgreenShift;
uniform float sdblueShift;
uniform float hhredShift;
uniform float hhgreenShift;
uniform float hhblueShift;
uniform float temperature;
uniform float tint;

#endif

const vec3 warmFilter = vec3(0.93, 0.54, 0.0);
const mat3 RGBtoYIQ = mat3(0.299, 0.587, 0.114, 0.596, -0.274, -0.322, 0.212, -0.523, 0.311);
const mat3 YIQtoRGB = mat3(1.0, 0.956, 0.621, 1.0, -0.272, -0.647, 1.0, -1.105, 1.702);
const vec3 luminanceWeighting = vec3(0.3, 0.3, 0.3);
const vec3 saturationWeighting = vec3(0.2125, 0.7154, 0.0721);

vec4 fhightshadows(vec4 rgba){
    
    float t_shadows=abs(shadows);
    float t_highlights=0.0;
    if(highlights>0.0){
        t_highlights=1.0-highlights;
    }else{
        t_highlights=1.0+highlights;
    }
    float luminance = dot(rgba.rgb, luminanceWeighting);
    float shadow=0.0;
    float highlight=0.0;
    shadow = clamp((pow(luminance, 1.0/(t_shadows+1.0)) + (-0.76)*pow(luminance, 2.0/(t_shadows+1.0))) - luminance, 0.0, 1.0);
    highlight = clamp((1.0 - (pow(1.0-luminance, 1.0/(2.0-t_highlights)) + (-0.8)*pow(1.0-luminance, 2.0/(2.0-t_highlights)))) - luminance, -1.0, 0.0);
    if(shadows<=0.0){
        shadow=-1.0*shadow;
    }
    
    if(highlights>0.0){
        highlight=-1.0*highlight;
    }
    
    float flagh=step(0.0,highlights);
    vec3 result = vec3(0.0, 0.0, 0.0) + ((luminance +shadow+highlight) - 0.0) * ((rgba.rgb - vec3(0.0, 0.0, 0.0))/(luminance - 0.0));
    return vec4(result.rgb, rgba.a);
}

vec4 fcontrast(vec4 rgba){
    return vec4(((rgba.rgb - vec3(0.5)) * (contrast+1.0) + vec3(0.5)), rgba.w);
}

vec4 fsaturation(vec4 rgba){
#ifdef GL_ES
    highp float luminance = dot(rgba.rgb, saturationWeighting);
    highp vec3 greyScaleColor = vec3(luminance);
    return vec4(mix(greyScaleColor, rgba.rgb, saturation+1.0), rgba.w);
#else
    float luminance = dot(rgba.rgb, saturationWeighting);
    vec3 greyScaleColor = vec3(luminance);
    return vec4(mix(greyScaleColor, rgba.rgb, saturation+1.0), rgba.w);
#endif
}

vec4 fbrightness(vec4 rgba){
    vec4 src = rgba;
    float fac = 1.0 / (brightness+0.1);
    float fac2 = 1.0 - fac;
    vec3 tmp1 = fac2 - src.rgb;
    vec3 tmp2 = sqrt(fac2 * fac2 + (4.0 * fac) * src.rgb);
    src.rgb = tmp1 + tmp2 * (step(0.0, brightness+0.01) * 2.0 - 1.0);
    return src;
}

vec4 fwhitening(vec4 rgba){
#ifdef GL_ES
    highp vec4 textureColor =rgba;
    highp float blueColor = textureColor.b * 63.0;
    highp vec2 quad1;
    quad1.y = floor(floor(blueColor) / 8.0);
    quad1.x = floor(blueColor) - (quad1.y * 8.0);
    highp vec2 quad2;
    quad2.y = floor(ceil(blueColor) /8.0);
    quad2.x = ceil(blueColor) - (quad2.y * 8.0);
    highp vec2 texPos1;
    texPos1.x = (quad1.x * 1.0/8.0)+ 0.5/512.0 + ((1.0/8.0 - 1.0/512.0) * textureColor.r);
    texPos1.y = (quad1.y * 1.0/8.0) + ((1.0/8.0 - 1.0/512.0) * textureColor.g);
    highp vec2 texPos2;
    texPos2.x = (quad2.x * 1.0/8.0)+ 0.5/512.0 + ((1.0/8.0 - 1.0/512.0) * textureColor.r);
    texPos2.y = (quad2.y * 1.0/8.0)+ 0.5/512.0 + ((1.0/8.0 - 1.0/512.0) * textureColor.g);
    highp vec4 newColor1 = texture2D(aTexture1, texPos1);
    highp vec4 newColor2 = texture2D(aTexture1, texPos2);
    highp vec4 newColor = mix(newColor1, newColor2, fract(blueColor));
    return mix(textureColor, vec4(newColor.rgb, textureColor.w),whitening);
#else
    vec4 textureColor =rgba;
    float blueColor = textureColor.b * 63.0;
    vec2 quad1;
    quad1.y = floor(floor(blueColor) / 8.0);
    quad1.x = floor(blueColor) - (quad1.y * 8.0);
    vec2 quad2;
    quad2.y = floor(ceil(blueColor) /8.0);
    quad2.x = ceil(blueColor) - (quad2.y * 8.0);
    vec2 texPos1;
    texPos1.x = (quad1.x * 1.0/8.0)+ 0.5/512.0 + ((1.0/8.0 - 1.0/512.0) * textureColor.r);
    texPos1.y = (quad1.y * 1.0/8.0) + ((1.0/8.0 - 1.0/512.0) * textureColor.g);
    vec2 texPos2;
    texPos2.x = (quad2.x * 1.0/8.0)+ 0.5/512.0 + ((1.0/8.0 - 1.0/512.0) * textureColor.r);
    texPos2.y = (quad2.y * 1.0/8.0)+ 0.5/512.0 + ((1.0/8.0 - 1.0/512.0) * textureColor.g);
    vec4 newColor1 = texture2D(aTexture1, texPos1);
    vec4 newColor2 = texture2D(aTexture1, texPos2);
    vec4 newColor = mix(newColor1, newColor2, fract(blueColor));
    return mix(textureColor, vec4(newColor.rgb, textureColor.w),whitening);
#endif
}

vec4 fgamma(vec4 rgba){
    return vec4(pow(rgba.rgb, vec3(gamma)), rgba.w);
}

float RGBToL(vec3 color){
    float fmin = min(min(color.r, color.g), color.b);    //Min. value of RGB
    float fmax = max(max(color.r, color.g), color.b);    //Max. value of RGB
    return (fmax + fmin) / 2.0; // Luminance
}

vec3 RGBToHSL(vec3 color){
    vec3 hsl; // init to 0 to avoid warnings ? (and reverse if + remove first part)
    float fmin = min(min(color.r, color.g), color.b);    //Min. value of RGB
    float fmax = max(max(color.r, color.g), color.b);    //Max. value of RGB
    float delta = fmax - fmin;             //Delta RGB value
    hsl.z = (fmax + fmin) / 2.0; // Luminance
    if (delta == 0.0){       //This is a gray, no chroma...
        hsl.x = 0.0;    // Hue
        hsl.y = 0.0;    // Saturation
    }else{                                    //Chromatic data...
        if (hsl.z < 0.5)
            hsl.y = delta / (fmax + fmin); // Saturation
        else
            hsl.y = delta / (2.0 - fmax - fmin); // Saturation
        float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;
        float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;
        float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;
        if (color.r == fmax )
            hsl.x = deltaB - deltaG; // Hue
        else if (color.g == fmax)
            hsl.x = (1.0 / 3.0) + deltaR - deltaB; // Hue
        else if (color.b == fmax)
            hsl.x = (2.0 / 3.0) + deltaG - deltaR; // Hue
        
        if (hsl.x < 0.0)
            hsl.x += 1.0; // Hue
        else if (hsl.x > 1.0)
            hsl.x -= 1.0; // Hue
    }
    return hsl;
}

float HueToRGB(float f1, float f2, float hue){
    if (hue < 0.0)
        hue += 1.0;
    else if (hue > 1.0)
        hue -= 1.0;
    float res;
    if ((6.0 * hue) < 1.0)
        res = f1 + (f2 - f1) * 6.0 * hue;
    else if ((2.0 * hue) < 1.0)
        res = f2;
    else if ((3.0 * hue) < 2.0)
        res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;
    else
        res = f1;
    return res;
}

vec3 HSLToRGB(vec3 hsl){
    vec3 rgb;
    if (hsl.y == 0.0)
        rgb = vec3(hsl.z); // Luminance
    else{
        float f2;
        if (hsl.z < 0.5)
            f2 = hsl.z * (1.0 + hsl.y);
        else
            f2 = (hsl.z + hsl.y) - (hsl.y * hsl.z);
        float f1 = 2.0 * hsl.z - f2;
        rgb.r = HueToRGB(f1, f2, hsl.x + (1.0/3.0));
        rgb.g = HueToRGB(f1, f2, hsl.x);
        rgb.b= HueToRGB(f1, f2, hsl.x - (1.0/3.0));
    }
    return rgb;
}

vec4 fcolorBalance(vec4 rgba){
    vec4 textureColor = rgba;
    // New way:
    float lightness = RGBToL(textureColor.rgb);
    vec3 shift = vec3(redShift*0.5, greenShift*0.5, blueShift*0.5);
    vec3 shiftShadows = vec3(sdredShift*0.5, sdgreenShift*0.5, sdblueShift*0.5);
    vec3 shiftHighlights = vec3(hhredShift*0.5, hhgreenShift*0.5, hhblueShift*0.5);
    const float a = 0.55;
    const float b = 0.5;
    const float scale = 0.7;
    
    vec3 shadows = (clamp((lightness - 0.33333) / -0.25 + 0.5, 0.0, 1.0)* 0.5) * shiftShadows;
    vec3 midtones = (clamp((lightness - b) /  a + 0.5, 0.0, 1.0) * clamp ((lightness + b - 1.0) / -a + 0.5, 0.0, 1.0) * scale) * shift;
    vec3 highlights =  (clamp((lightness -  0.33333) / 0.25 + 0.5, 0.0, 1.0)* 0.15)  * shiftHighlights;
    vec3 newColor = textureColor.rgb + shadows;
    newColor=newColor+midtones;
    newColor=newColor+highlights;
    newColor = clamp(newColor, 0.0, 1.0);
    // preserve luminosity
    vec3 newHSL = RGBToHSL(newColor);
    float oldLum = RGBToL(textureColor.rgb);
    textureColor.rgb = HSLToRGB(vec3(newHSL.x, newHSL.y, oldLum));
    return textureColor;
}

vec4 fwhiteBalance(vec4 rgba){
    vec4 source = rgba;
    vec3 yiq = RGBtoYIQ * source.rgb; //adjusting tint
    yiq.b = clamp(yiq.b + tint*0.5226*0.1, -0.5226, 0.5226);
    vec3 rgb = YIQtoRGB * yiq;
    vec3 processed = vec3(
                          (rgb.r < 0.5 ? (2.0 * rgb.r * warmFilter.r) : (1.0 - 2.0 * (1.0 - rgb.r) * (1.0 - warmFilter.r))), //adjusting temperature
                          (rgb.g < 0.5 ? (2.0 * rgb.g * warmFilter.g) : (1.0 - 2.0 * (1.0 - rgb.g) * (1.0 - warmFilter.g))),
                          (rgb.b < 0.5 ? (2.0 * rgb.b * warmFilter.b) : (1.0 - 2.0 * (1.0 - rgb.b) * (1.0 - warmFilter.b))));
    return vec4(mix(rgb, processed, temperature), source.a);
}

void main(){
   vec4 color=texture2D(aTexture0, v_texcoord0);
    if(temperature!=0.0){
        color=fwhiteBalance(color);
    }
    if(gamma!=0.0){
        color = fgamma(color);
    }
    if(redShift!=0.0||greenShift!=0.0||blueShift!=0.0||sdredShift!=0.0||sdgreenShift!=0.0||sdblueShift!=0.0||hhredShift!=0.0||hhgreenShift!=0.0||hhblueShift!=0.0){
         color=fcolorBalance(color);
    }
    if(whitening!=0.0){
        color = fwhitening(color);
    }
    if(shadows!=0.0||highlights!=0.0){
        color = fhightshadows(color);
    }
    if(contrast!=0.0){
        color = fcontrast(color);
    }
    if(saturation!=0.0){
        color = fsaturation(color);
    }
    if(brightness!=0.0){
        color = fbrightness(color);
     }
    gl_FragColor=color;
}
