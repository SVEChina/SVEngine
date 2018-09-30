precision highp float;

uniform float               iGlobalTime;
uniform sampler2D           aTexture0;
varying vec2                v_texcoord0;

float remap(float value, float inputMin, float inputMax, float outputMin, float outputMax)
{
    return (value - inputMin) * ((outputMax - outputMin) / (inputMax - inputMin)) + outputMin;
}

void main() {
    vec2 uv = v_texcoord0;
    float normalizedContrast = 1.0;
    float contrast = remap(normalizedContrast, 0.0, 1.0, 0.2 /*min*/, 4.0 /*max*/);
    vec4 srcColor = texture2D(aTexture0, uv);
    vec4 dstColor = vec4((srcColor.rgb - vec3(0.5)) * contrast + vec3(0.5), 1.0);
    gl_FragColor = clamp(dstColor, 0.0, 1.0);
}
