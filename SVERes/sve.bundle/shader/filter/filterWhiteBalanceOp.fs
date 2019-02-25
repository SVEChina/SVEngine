#ifdef GL_ES
precision mediump float;
#endif

//白平衡优化

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec3 balance;

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
    src.rgb = dispatch(src.rgb * balance);
    gl_FragColor = src;
}
