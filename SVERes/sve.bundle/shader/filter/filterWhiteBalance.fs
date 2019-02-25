#ifdef GL_ES
precision mediump float;
#endif

//白平衡
varying vec2 v_texcoord0;
uniform sampler2D aTexture0;

uniform float temperature;
uniform float tint;

vec3 whiteBalance(vec3 src, float temp, float tint)
{
    temp = clamp(temp, 1200.0, 12000.0);
    tint = clamp(tint, 0.02, 5.0);
    float xD;
    temp /= 1000.0;
    
    if(temp < 4.0)
        xD = 0.27475 / (temp * temp * temp) - 0.98598 / (temp * temp) + 1.17444 / temp + 0.145986;
    else if(temp < 7.0)
        xD = -4.6070 / (temp * temp * temp) + 2.9678 / (temp * temp) + 0.09911 / temp + 0.244063;
    else xD = -2.0064 / (temp * temp * temp) + 1.9018 / (temp * temp) + 0.24748 / temp + 0.237040;
    
    float yD = -3.0 * xD * xD + 2.87 * xD - 0.275;
    
    float X = xD / yD;
    float Z = (1.0 - xD - yD) / yD;
    
    vec3 color;
    
    color.r = X * 3.24074 - 1.53726 - Z * 0.498571;
    color.g = -X * 0.969258 + 1.87599 + Z * 0.0415557;
    color.b = X * 0.0556352 - 0.203996 + Z * 1.05707;
    
    color.g /= tint;
    
    color /= max(max(color.r, color.g), color.b);
    color = 1.0 / color;
    color /= color.r * 0.299 + color.g * 0.587 + color.b * 0.114;
    
    return src * color;
}

vec3 map_color(vec3 src, float lum)
{
    vec3 h = src - lum;
    if(src.r > 1.0)
    {
        float tmp = 1.0 - lum;
        h.g = h.g * tmp / h.r;
        h.b = h.b * tmp / h.r;
        h.r = tmp;
    }
    float t3r = h.b + lum;
    if(t3r < -0.00003)
    {
        src.rg = lum - h.rg * lum / h.b;
        src.b = 0.0;
    }
    else
    {
        src.rg = lum + h.rg;
        src.b = t3r;
    }
    return src;
}

vec3 dispatch(vec3 src)
{
    float lum = dot(src, vec3(0.299, 0.587, 0.114));
    if(src.g > src.b)
    {
        if(src.r > src.g)
        {
            src = map_color(src, lum);
        }
        else if(src.r > src.b)
        {
            src.grb = map_color(src.grb, lum);
        }
        else
        {
            src.gbr = map_color(src.gbr, lum);
        }
    }
    else
    {
        if(src.g > src.r)
        {
            src.bgr = map_color(src.bgr, lum);
        }
        else if(src.b > src.r)
        {
            src.brg = map_color(src.brg, lum);
        }
        else
        {
            src.rbg = map_color(src.rbg, lum);
        }
    }
    return src;
}

void main()
{
    vec4 src = texture2D(aTexture0, v_texcoord0);
    src.rgb = dispatch(whiteBalance(src.rgb, temperature, tint));
    gl_FragColor = src;
}
