#ifdef GL_ES

precision mediump float;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
varying highp vec2 v_texcoord0;
const lowp int GAUSSIAN_SAMPLES = 9;
varying highp vec2 blurCoordinates[GAUSSIAN_SAMPLES];
uniform mediump float distanceNormalizationFactor;
uniform mediump float hlafWidth;
uniform mediump float hlafHeight;

#else

const int GAUSSIAN_SAMPLES = 9;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
varying vec2 v_texcoord0;
varying vec2 blurCoordinates[GAUSSIAN_SAMPLES];
uniform float distanceNormalizationFactor;
uniform float hlafWidth;
uniform float hlafHeight;

#endif



void main()
{
#ifdef GL_ES
    lowp vec4 centralColor;
    lowp float gaussianWeightTotal;
    lowp vec4 sum;
    lowp vec4 sampleColor;
    lowp float distanceFromCentralColor;
    lowp float gaussianWeight;
#else
    vec4 centralColor;
    float gaussianWeightTotal;
    vec4 sum;
    vec4 sampleColor;
    float distanceFromCentralColor;
    float gaussianWeight;
#endif
    
    centralColor = texture2D(aTexture0, blurCoordinates[4]);
    gaussianWeightTotal = 0.18;
    sum = centralColor * 0.18;
    
    sampleColor = texture2D(aTexture0, blurCoordinates[0]);
    distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.05 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sampleColor * gaussianWeight;
    
    sampleColor = texture2D(aTexture0, blurCoordinates[1]);
    distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.09 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sampleColor * gaussianWeight;
    
    sampleColor = texture2D(aTexture0, blurCoordinates[2]);
    distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.12 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sampleColor * gaussianWeight;
    
    sampleColor = texture2D(aTexture0, blurCoordinates[3]);
    distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.15 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sampleColor * gaussianWeight;
    
    sampleColor = texture2D(aTexture0, blurCoordinates[5]);
    distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.15 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sampleColor * gaussianWeight;
    
    sampleColor = texture2D(aTexture0, blurCoordinates[6]);
    distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.12 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sampleColor * gaussianWeight;
    
    sampleColor = texture2D(aTexture0, blurCoordinates[7]);
    distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.09 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sampleColor * gaussianWeight;
    
    sampleColor = texture2D(aTexture0, blurCoordinates[8]);
    distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.05 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sampleColor * gaussianWeight;
    
    
    vec4 src=texture2D(aTexture1, v_texcoord0);
    vec4 dst;
    //gl_FragColor=src;
    if (gaussianWeightTotal < 0.4)
    {
        dst = centralColor;
    }
    else if (gaussianWeightTotal < 0.5)
    {
        dst = mix(sum / gaussianWeightTotal, centralColor, (gaussianWeightTotal - 0.4) / 0.1);
    }
    else
    {
        dst = sum / gaussianWeightTotal;
    }
    
    
    gl_FragColor=dst-src+0.5;
}
