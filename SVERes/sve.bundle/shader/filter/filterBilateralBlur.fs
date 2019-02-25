#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;

//const int GAUSSIAN_SAMPLES = 9;
uniform float blurFactors[9];// = float[GAUSSIAN_SAMPLES](0.05, 0.09, 0.12, 0.15, 0.18, 0.15, 0.12, 0.09, 0.05);

uniform float distanceNormalizationFactor;
uniform float blurSamplerScale;
uniform vec2 samplerSteps;

const int samplerRadius = 4;

float random(vec2 seed)
{
    return fract(sin(dot(seed ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    vec4 centralColor = texture2D(aTexture0, v_texcoord0);
    float gaussianWeightTotal = blurFactors[4];
    vec4 sum = centralColor * blurFactors[4];
    vec2 stepScale = blurSamplerScale * samplerSteps;
    float offset = random(v_texcoord0) - 0.5;
    
    for(int i = 0; i < samplerRadius; ++i)
    {
        vec2 dis = (float(i) + offset) * stepScale;
        
        float blurfactor = blurFactors[samplerRadius-i];
        
        {
            vec4 sampleColor1 = texture2D(aTexture0, v_texcoord0 + dis);
            float distanceFromCentralColor1 = min(distance(centralColor, sampleColor1) * distanceNormalizationFactor, 1.0);
            float gaussianWeight1 = blurfactor * (1.0 - distanceFromCentralColor1);
            gaussianWeightTotal += gaussianWeight1;
            sum += sampleColor1 * gaussianWeight1;
        }
        
        //////////////////////////////////////////////////////////////////////////
        
        {
            vec4 sampleColor2 = texture2D(aTexture0, v_texcoord0 - dis);
            float distanceFromCentralColor2 = min(distance(centralColor, sampleColor2) * distanceNormalizationFactor, 1.0);
            float gaussianWeight2 = blurfactor * (1.0 - distanceFromCentralColor2);
            gaussianWeightTotal += gaussianWeight2;
            sum += sampleColor2 * gaussianWeight2;
        }
    }
    
    gl_FragColor = sum / gaussianWeightTotal;
}
