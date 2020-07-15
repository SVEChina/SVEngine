precision lowp float;
uniform sampler2D aTexture0;
varying lowp vec2 v_texcoord0;
uniform float softenStrength;
uniform float white;


void main(){
    
    vec3 centralColor;
    
    float mul_x = 2.0 / 720.0;
    float mul_y = 2.0 / 1280.0;
    
    vec2 blurCoordinates0 = v_texcoord0 + vec2(0.0 * mul_x,-10.0 * mul_y);
    vec2 blurCoordinates1 = v_texcoord0 + vec2(5.0 * mul_x,-8.0 * mul_y);
    vec2 blurCoordinates2 = v_texcoord0 + vec2(8.0 * mul_x,-5.0 * mul_y);
    vec2 blurCoordinates3 = v_texcoord0 + vec2(10.0 * mul_x,0.0 * mul_y);
    vec2 blurCoordinates4 = v_texcoord0 + vec2(8.0 * mul_x,5.0 * mul_y);
    vec2 blurCoordinates5 = v_texcoord0 + vec2(5.0 * mul_x,8.0 * mul_y);
    vec2 blurCoordinates6 = v_texcoord0 + vec2(0.0 * mul_x,10.0 * mul_y);
    vec2 blurCoordinates7 = v_texcoord0 + vec2(-5.0 * mul_x,8.0 * mul_y);
    vec2 blurCoordinates8 = v_texcoord0 + vec2(-8.0 * mul_x,5.0 * mul_y);
    vec2 blurCoordinates9 = v_texcoord0 + vec2(-10.0 * mul_x,0.0 * mul_y);
    vec2 blurCoordinates10 = v_texcoord0 + vec2(-8.0 * mul_x,-5.0 * mul_y);
    vec2 blurCoordinates11 = v_texcoord0 + vec2(-5.0 * mul_x,-8.0 * mul_y);
    
    mul_x = 1.6 / 720.0;
    mul_y = 1.6 / 1080.0;
    
    vec2 blurCoordinates12 = v_texcoord0 + vec2(0.0 * mul_x,-6.0 * mul_y);
    vec2 blurCoordinates13 = v_texcoord0 + vec2(-4.0 * mul_x,-4.0 * mul_y);
    vec2 blurCoordinates14 = v_texcoord0 + vec2(-6.0 * mul_x,0.0 * mul_y);
    vec2 blurCoordinates15 = v_texcoord0 + vec2(-4.0 * mul_x,4.0 * mul_y);
    vec2 blurCoordinates16 = v_texcoord0 + vec2(0.0 * mul_x,6.0 * mul_y);
    vec2 blurCoordinates17 = v_texcoord0 + vec2(4.0 * mul_x,4.0 * mul_y);
    vec2 blurCoordinates18 = v_texcoord0 + vec2(6.0 * mul_x,0.0 * mul_y);
    vec2 blurCoordinates19 = v_texcoord0 + vec2(4.0 * mul_x,-4.0 * mul_y);
    
    float central;
    float gaussianWeightTotal;
    float sum;
    float sample;
    float distanceFromCentralColor;
    float gaussianWeight;
    
    float distanceNormalizationFactor = 3.6;
    
    central = texture2D(aTexture0, v_texcoord0).g;
    gaussianWeightTotal = 0.2;
    sum = central * 0.2;
    
    sample = texture2D(aTexture0, blurCoordinates0).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates1).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates2).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates3).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates4).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates5).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates6).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates7).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates8).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates9).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates10).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates11).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates12).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates13).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates14).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates15).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates16).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates17).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates18).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(aTexture0, blurCoordinates19).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sum = sum/gaussianWeightTotal;
    
    centralColor = texture2D(aTexture0, v_texcoord0).rgb;
    
    sample = centralColor.g - sum + 0.5;
    
    for(int i = 0; i < 5; ++i)
    {
        if(sample <= 0.5)
        {
            sample = sample * sample * 2.0;
        }
        else
        {
            sample = 1.0 - ((1.0 - sample)*(1.0 - sample) * 2.0);
        }
    }
    
    float aa = 1.0 + pow(sum, 0.3)*0.09;
    vec3 smoothColor = centralColor*aa - vec3(sample)*(aa-1.0);// get smooth color
    smoothColor = clamp(smoothColor,vec3(0.0),vec3(1.0));//make smooth color right
    
    smoothColor = mix(centralColor, smoothColor, pow(centralColor.g, 0.33));
    smoothColor = mix(centralColor, smoothColor, pow(centralColor.g, 0.39));
    
    smoothColor = mix(centralColor, smoothColor, softenStrength);
    gl_FragColor = vec4(smoothColor,1.0);
}
