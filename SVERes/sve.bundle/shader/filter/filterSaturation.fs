#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float saturation;
uniform float hue;
uniform float luminance;

vec3 RGB2HSL(vec3 src)
{
    float maxc = max(max(src.r, src.g), src.b);
    float minc = min(min(src.r, src.g), src.b);
    float L = (maxc + minc) / 2.0;
    if(maxc == minc)
        return vec3(0.0, 0.0, L);
    float H, S;
    
    //////// Optimize ////////////////////////
    
    // 	if(L < 0.5)
    // 		S = (maxc - minc) / (maxc + minc);
    // 	else
    //		S = (maxc - minc) / (2.0 - maxc - minc);
    
    //注意， 某些低精度情况下 N - (A+B) != N - A - B
    float temp1 = maxc - minc;
    S = mix(temp1 / (2.0 - maxc - minc), temp1 / (maxc + minc), step(L, 0.5));
    
    //////// Optimize ////////////////////////
    
    // 	if(maxc == src.r)
    // 		H = (src.g - src.b) / (maxc - minc);
    // 	else if(maxc == src.g)
    // 		H = 2.0 + (src.b - src.r) / (maxc - minc);
    // 	else
    // 		H = 4.0 + (src.r - src.g) / (maxc - minc);
    
    vec3 comp;
    comp.xy = vec2(equal(src.xy, vec2(maxc)));
    float comp_neg = 1.0 - comp.x;
    comp.y *= comp_neg;
    comp.z = (1.0 - comp.y) * comp_neg;
    
    float dif = maxc - minc;
    vec3 result = comp * vec3((src.g - src.b) / dif,
                              2.0 + (src.b - src.r) / dif,
                              4.0 + (src.r - src.g) / dif);
    
    H = result.x + result.y + result.z;
    
    H *= 60.0;
    //if(H < 0.0) H += 360.0;
    H += step(H, 0.0) * 360.0;
    return vec3(H / 360.0, S, L); // H(0~1), S(0~1), L(0~1)
}

vec3 HSL2RGB(vec3 src) // H, S, L
{
    // 	if(src.y <= 0.0)
    // 		return vec3(src.z, src.z, src.z);
    float q = (src.z < 0.5) ? src.z * (1.0 + src.y) : (src.z + src.y - (src.y * src.z));
    float p = 2.0 * src.z - q;
    
    vec3 dst = vec3(src.x + 0.333, src.x, src.x - 0.333);
    
    //////// Optimize ////////////////////////
    
    // 	if(dst.r < 0.0) dst.r += 1.0;
    // 	else if(dst.r > 1.0) dst.r -= 1.0;
    //
    // 	if(dst.g < 0.0) dst.g += 1.0;
    // 	else if(dst.g > 1.0) dst.g -= 1.0;
    //
    // 	if(dst.b < 0.0) dst.b += 1.0;
    // 	else if(dst.b > 1.0) dst.b -= 1.0;
    
    //dst += step(dst, vec3(0.0));
    //dst -= step(vec3(1.0), dst);
    dst = fract(dst);
    
    //////// Optimize ////////////////////////
    
    //Plan A
    
    // 	if(dst.r < 1.0 / 6.0)
    // 		dst.r = p + (q - p) * 6.0 * dst.r;
    // 	else if(dst.r < 0.5)
    // 		dst.r = q;
    // 	else if(dst.r < 2.0 / 3.0)
    // 		dst.r = p + (q - p) * ((2.0 / 3.0) - dst.r) * 6.0;
    // 	else dst.r = p;
    //
    // 	if(dst.g < 1.0 / 6.0)
    // 		dst.g = p + (q - p) * 6.0 * dst.g;
    // 	else if(dst.g < 0.5)
    // 		dst.g = q;
    // 	else if(dst.g < 2.0 / 3.0)
    // 		dst.g = p + (q - p) * ((2.0 / 3.0) - dst.g) * 6.0;
    // 	else dst.g = p;
    //
    // 	if(dst.b < 1.0 / 6.0)
    // 		dst.b = p + (q - p) * 6.0 * dst.b;
    // 	else if(dst.b < 0.5)
    // 		dst.b = q;
    // 	else if(dst.b < 2.0 / 3.0)
    // 		dst.b = p + (q - p) * ((2.0 / 3.0) - dst.b) * 6.0;
    // 	else dst.b = p;
    
    //Plan B
    
    vec3 weight = step(dst, vec3(1.0 / 6.0));
    vec3 weight_neg = 1.0 - weight;
    
    vec3 weight2 = weight_neg * step(dst, vec3(0.5));
    vec3 weight2_neg = weight_neg * (1.0 - weight2);
    
    vec3 weight3 = weight2_neg * step(dst, vec3(2.0 / 3.0));
    vec3 weight4 = (1.0 - weight3) * weight2_neg;
    
    float q_p = q - p;
    
    dst = mix(dst, p + q_p * 6.0 * dst, weight);
    dst = mix(dst, vec3(q), weight2);
    dst = mix(dst, p + q_p * ((2.0 / 3.0) - dst) * 6.0, weight3);
    dst = mix(dst, vec3(p), weight4);
    
    return dst;
}

vec3 adjustColor(vec3 src, float h, float s, float l) //hue should be positive
{
    src = RGB2HSL(src);
    src.x += h;
    src.y *= 1.0 + s;
    src.z *= 1.0 + l;
    return HSL2RGB(src);
}

void main()
{
    vec4 src = texture2D(aTexture0, v_texcoord0);
    src.rgb = adjustColor(src.rgb, hue, saturation, luminance);
    gl_FragColor = src;
}
