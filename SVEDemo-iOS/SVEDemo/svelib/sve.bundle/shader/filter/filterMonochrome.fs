#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;

uniform float red;
uniform float green;
uniform float blue;
uniform float cyan;
uniform float magenta;
uniform float yellow;

void main()
{
    vec4 src = texture2D(aTexture0, v_texcoord0);
    
    float maxc = max(max(src.r, src.g), src.b);
    float minc = min(min(src.r, src.g), src.b);
    float midc = src.r + src.g + src.b - maxc - minc;
    
    if(maxc == minc)
        gl_FragColor = src;
    
    vec3 ratioMax, ratioMin;
    ratioMax.xy = vec2(equal(src.rg, vec2(maxc)));
    float max_neg = 1.0 - ratioMax.x;
    ratioMax.y *= max_neg;
    ratioMax.z = (1.0 - ratioMax.y) * max_neg;
    
    vec3 compMax = vec3(red, green, blue) * ratioMax;
    
    ratioMin.xy = vec2(equal(src.rg, vec2(minc)));
    float min_neg = 1.0 - ratioMin.x;
    ratioMin.y *= min_neg;
    ratioMin.z = (1.0 - ratioMin.y) * min_neg;
    
    vec3 compMaxMid = vec3(cyan, magenta, yellow) * ratioMin;
    
    float total = (compMax.x + compMax.y + compMax.z) * (maxc - midc) + (compMaxMid.x + compMaxMid.y + compMaxMid.z) * (midc - minc) + minc;
    
    gl_FragColor = vec4(total, total, total, 1.0);
}
