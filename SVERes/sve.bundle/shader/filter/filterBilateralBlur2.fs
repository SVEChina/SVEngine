#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;

uniform float distanceNormalizationFactor;
uniform float blurSamplerScale;
uniform vec2 samplerSteps;
uniform int samplerRadius;

const float arg = 0.5;

float random(vec2 seed)
{
    return fract(sin(dot(seed ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    vec4 centralColor = texture2D(aTexture0, v_texcoord0);
    float lum = dot(centralColor.rgb, vec3(0.299, 0.587, 0.114));
    float factor = (1.0 + arg) / (arg + lum) * distanceNormalizationFactor;
    
    float gaussianWeightTotal = 1.0;
    vec4 sum = centralColor * gaussianWeightTotal;
    vec2 stepScale = blurSamplerScale * samplerSteps / float(samplerRadius);
    float offset = random(v_texcoord0) - 0.5;
    
    for(int i = 1; i <= samplerRadius; ++i)
    {
        vec2 dis = (float(i) + offset) * stepScale;
        float percent = 1.0 - (float(i) + offset) / float(samplerRadius);
        
        {
            vec4 sampleColor1 = texture2D(aTexture0, v_texcoord0 + dis);
            float distanceFromCentralColor1 = min(distance(centralColor, sampleColor1) * factor, 1.0);
            float gaussianWeight1 = percent * (1.0 - distanceFromCentralColor1);
            gaussianWeightTotal += gaussianWeight1;
            sum += sampleColor1 * gaussianWeight1;
        }
        
        //////////////////////////////////////////////////////////////////////////
        
        {
            vec4 sampleColor2 = texture2D(aTexture0, v_texcoord0 - dis);
            float distanceFromCentralColor2 = min(distance(centralColor, sampleColor2) * factor, 1.0);
            float gaussianWeight2 = percent * (1.0 - distanceFromCentralColor2);
            gaussianWeightTotal += gaussianWeight2;
            sum += sampleColor2 * gaussianWeight2;
        }
    }
    
    gl_FragColor = sum / gaussianWeightTotal;
}
