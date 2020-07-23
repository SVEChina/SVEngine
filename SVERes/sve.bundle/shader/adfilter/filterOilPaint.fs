#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
varying vec2 v_position;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform vec2 tex0size;
const int kuwahara_radius = 4;
const float fade = 0.86;
const float desat = 0.3;
const float scale = 0.009;
const float intensity = 0.041;

vec4 kuwahara(vec2 uv, int radius){
    vec2 src_size = vec2 (1.0 / tex0size.x, 1.0 / tex0size.y);
    float n = float((radius + 1) * (radius + 1));
    int i;
    int j;
    vec3 m0 = vec3(0.0); vec3 m1 = vec3(0.0); vec3 m2 = vec3(0.0); vec3 m3 = vec3(0.0);
    vec3 s0 = vec3(0.0); vec3 s1 = vec3(0.0); vec3 s2 = vec3(0.0); vec3 s3 = vec3(0.0);
    vec3 c;

    for (int j = -radius; j <= 0; ++j)  {
        for (int i = -radius; i <= 0; ++i)  {
            c = texture2D(aTexture0, uv + vec2(i,j) * src_size).rgb;
            m0 += c;
            s0 += c * c;
        }
    }

    for (int j = -radius; j <= 0; ++j)  {
        for (int i = 0; i <= radius; ++i)  {
            c = texture2D(aTexture0, uv + vec2(i,j) * src_size).rgb;
            m1 += c;
            s1 += c * c;
        }
    }

    for (int j = 0; j <= radius; ++j)  {
        for (int i = 0; i <= radius; ++i)  {
            c = texture2D(aTexture0, uv + vec2(i,j) * src_size).rgb;
            m2 += c;
            s2 += c * c;
        }
    }

    for (int j = 0; j <= radius; ++j)  {
        for (int i = -radius; i <= 0; ++i)  {
            c = texture2D(aTexture0, uv + vec2(i,j) * src_size).rgb;
            m3 += c;
            s3 += c * c;
        }
    }


    float min_sigma2 = 1e+2;
    m0 /= n;
    s0 = abs(s0 / n - m0 * m0);
    vec4 color;
    float sigma2 = s0.r + s0.g + s0.b;
    if (sigma2 < min_sigma2) {
        min_sigma2 = sigma2;
        color = vec4(m0, 1.0);
    }

    m1 /= n;
    s1 = abs(s1 / n - m1 * m1);

    sigma2 = s1.r + s1.g + s1.b;
    if (sigma2 < min_sigma2) {
        min_sigma2 = sigma2;
        color = vec4(m1, 1.0);
    }

    m2 /= n;
    s2 = abs(s2 / n - m2 * m2);

    sigma2 = s2.r + s2.g + s2.b;
    if (sigma2 < min_sigma2) {
        min_sigma2 = sigma2;
        color = vec4(m2, 1.0);
    }

    m3 /= n;
    s3 = abs(s3 / n - m3 * m3);

    sigma2 = s3.r + s3.g + s3.b;
    if (sigma2 < min_sigma2) {
        min_sigma2 = sigma2;
        color = vec4(m3, 1.0);
    }
    return color;
}

void main(){
    vec2 uv = v_texcoord0;
//    vec2 distortion = texture2D(aTexture1, uv).rg*2.0-1.0;
//    vec2 distortion = texture2D(aTexture1, v_texcoord0).rg/*triplanarMapping(distort_texture, world_normal, world_position * scale).rg*/ * 2.0 - 1.0;
//    uv += intensity * distortion;
    vec3 paint = kuwahara(uv, kuwahara_radius).rgb;
//    float desaturation = texture2D(aTexture1, v_texcoord0).g * desat;
//    paint = mix(paint, vec3(paint.r + paint.g + paint.b) / 3.0, desaturation);
//
//    gl_FragColor = vec4(mix(texture2D(aTexture0, v_texcoord0).rgb, paint, fade), 1.0);
    
    gl_FragColor = vec4(paint, 1.0);
}
