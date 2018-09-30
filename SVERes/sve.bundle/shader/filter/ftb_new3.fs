precision highp float;
varying highp vec2 v_texcoord0;
uniform sampler2D aTexture0;

vec3 AdjustWhiteColor(vec3 inputColor, vec4 adjustAmount, float threshold)
{
    vec3 outputColor = inputColor;
    outputColor.r = outputColor.r - adjustAmount.r * (threshold - 0.66);
    outputColor.g = outputColor.g - adjustAmount.g * (threshold - 0.66);
    outputColor.b = outputColor.b - adjustAmount.b * (threshold - 0.66);
    return outputColor;
}

vec3 AdjustBlackColor(vec3 inputColor, vec4 adjustAmount, float threshold)
{
    vec3 outputColor = inputColor;
    outputColor.r = outputColor.r - adjustAmount.r * (0.33 - threshold);
    outputColor.g = outputColor.g - adjustAmount.g * (0.33 - threshold);
    outputColor.b = outputColor.b - adjustAmount.b * (0.33 - threshold);
    return outputColor;
}

vec3 AdjustMiddleColor(vec3 inputColor, vec4 adjustAmount, float threshold)
{
    vec3 outputColor = inputColor;
    outputColor.r = outputColor.r - adjustAmount.r * 0.33;
    outputColor.g = outputColor.g - adjustAmount.g * 0.33;
    outputColor.b = outputColor.b - adjustAmount.b * 0.33;
    return outputColor;
}

lowp vec3 RGBToHSL(lowp vec3 color)
{
    lowp vec3 hsl; // init to 0 to avoid warnings ? (and reverse if + remove first part)
    lowp float fmin = min(min(color.r, color.g), color.b);    //Min. value of RGB
    lowp float fmax = max(max(color.r, color.g), color.b);    //Max. value of RGB
    lowp float delta = fmax - fmin;             //Delta RGB value
    hsl.z = (fmax + fmin) / 2.0; // Luminance
    if (delta == 0.0){
        //This is a gray, no chroma...
        hsl.x = 0.0;	// Hue
        hsl.y = 0.0;	// Saturation
    }
    else{
        //Chromatic data...
        if (hsl.z < 0.5)
            hsl.y = delta / (fmax + fmin); // Saturation
        else
            hsl.y = delta / (2.0 - fmax - fmin); // Saturation
        
        lowp float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;
        lowp float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;
        lowp float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;
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


lowp float HueToRGB(lowp float f1, lowp float f2, lowp float hue)
{
    if (hue < 0.0)
        hue += 1.0;
    else if (hue > 1.0)
        hue -= 1.0;
    lowp float res;
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


lowp vec3 HSLToRGB(lowp vec3 hsl)
{
    lowp vec3 rgb;
    if (hsl.y == 0.0){
        rgb = vec3(hsl.z); // Luminance
    }
    else
    {
        lowp float f2;
        if (hsl.z < 0.5)
            f2 = hsl.z * (1.0 + hsl.y);
        else
            f2 = (hsl.z + hsl.y) - (hsl.y * hsl.z);
        lowp float f1 = 2.0 * hsl.z - f2;
        rgb.r = HueToRGB(f1, f2, hsl.x + (1.0/3.0));
        rgb.g = HueToRGB(f1, f2, hsl.x);
        rgb.b= HueToRGB(f1, f2, hsl.x - (1.0/3.0));
    }
    return rgb;
}

lowp float RGBToL(lowp vec3 color)
{
    lowp float fmin = min(min(color.r, color.g), color.b);    //Min. value of RGB
    lowp float fmax = max(max(color.r, color.g), color.b);    //Max. value of RGB
    return (fmax + fmin) / 2.0; // Luminance
}

void main()
{
    mat3 saturateMatrix = mat3(
        1.1102,-0.0598,-0.061,
        -0.0774,1.0826,-0.1186,
        -0.0228,-0.0228,1.1772);

    mat3 brightMatrix = mat3(
        1.1,0.0,0.0,
        0.0,1.1,0.0,
        0.0,0.0,1.1);

    vec4 tmp_FragColor = texture2D(aTexture0,v_texcoord0);

    float dist = distance(v_texcoord0, vec2(0.5,0.5));
    dist = smoothstep(0.0,1.0,dist);
    dist = pow(dist, 1.5);
    vec4 darkenColor = gl_FragColor;
    darkenColor.r = pow(darkenColor.r, 3.0);
    darkenColor.g = pow(darkenColor.g, 3.0);
    darkenColor.b = pow(darkenColor.b, 3.0);

    tmp_FragColor = mix(tmp_FragColor, darkenColor, dist);

    //冷绿
    vec3 shadowsShift = vec3(-0.09, 0.06, 0.05);
    vec3 midtonesShift = vec3(0.0, 0.0, 0.0);
    vec3 highlightsShift = vec3(-0.13, -0.07, 0.01);
    int preserveLuminosity = 0;

    // Alternative way:
    //lowp vec3 lightness = RGBToL(gl_FragColor.rgb);
    lowp vec3 lightness = tmp_FragColor.rgb;
    const lowp float a = 0.25;
    const lowp float b = 0.333;
    const lowp float scale = 0.7;
    lowp vec3 shadows = shadowsShift * (clamp((lightness - b) / -a + 0.5, 0.0, 1.0) * scale);
    lowp vec3 midtones = midtonesShift * (clamp((lightness - b) / a + 0.5, 0.0, 1.0) * clamp((lightness + b - 1.0) / -a + 0.5, 0.0, 1.0) * scale);
    lowp vec3 highlights = highlightsShift * (clamp((lightness + b - 1.0) / a + 0.5, 0.0, 1.0) * scale);

    mediump vec3 newColor = clamp(tmp_FragColor.rgb + shadows + midtones + highlights, 0.0, 1.0);
    if (preserveLuminosity != 0) {
        lowp vec3 newHSL = RGBToHSL(newColor);
        lowp float oldLum = RGBToL(tmp_FragColor.rgb);
        gl_FragColor.rgb = HSLToRGB(vec3(newHSL.x, newHSL.y, oldLum));
    } else {
        gl_FragColor = vec4(newColor.rgb, tmp_FragColor.w);
    }
}
