precision lowp float;
uniform sampler2D inputImageTexture1;
uniform sampler2D inputImageTexture2;
varying lowp vec2 textureCoordinate;
uniform float smooth;
uniform float white;

vec4 lookup8x8(vec3 textureColor)
{
    highp float blueColor = textureColor.b * 63.0;
    
    highp vec2 quad1;
    quad1.y = floor(floor(blueColor) / 8.0);
    quad1.x = floor(blueColor) - (quad1.y * 8.0);
    
    highp vec2 texPos1;
    texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
    texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);
    
    lowp vec3 newColor1 = texture2D(inputImageTexture2, texPos1).rgb;
    
    lowp vec3 newColor = newColor1;
    return vec4(mix(textureColor, newColor, white), 1.0);
}

void main(){
    
    vec3 centralColor;
    
    float mul_x = 2.0 / 720.0;
    float mul_y = 2.0 / 1280.0;
    
    vec2 blurCoordinates0 = textureCoordinate + vec2(0.0 * mul_x,-10.0 * mul_y);
    vec2 blurCoordinates1 = textureCoordinate + vec2(5.0 * mul_x,-8.0 * mul_y);
    vec2 blurCoordinates2 = textureCoordinate + vec2(8.0 * mul_x,-5.0 * mul_y);
    vec2 blurCoordinates3 = textureCoordinate + vec2(10.0 * mul_x,0.0 * mul_y);
    vec2 blurCoordinates4 = textureCoordinate + vec2(8.0 * mul_x,5.0 * mul_y);
    vec2 blurCoordinates5 = textureCoordinate + vec2(5.0 * mul_x,8.0 * mul_y);
    vec2 blurCoordinates6 = textureCoordinate + vec2(0.0 * mul_x,10.0 * mul_y);
    vec2 blurCoordinates7 = textureCoordinate + vec2(-5.0 * mul_x,8.0 * mul_y);
    vec2 blurCoordinates8 = textureCoordinate + vec2(-8.0 * mul_x,5.0 * mul_y);
    vec2 blurCoordinates9 = textureCoordinate + vec2(-10.0 * mul_x,0.0 * mul_y);
    vec2 blurCoordinates10 = textureCoordinate + vec2(-8.0 * mul_x,-5.0 * mul_y);
    vec2 blurCoordinates11 = textureCoordinate + vec2(-5.0 * mul_x,-8.0 * mul_y);
    
    mul_x = 1.6 / 720.0;
    mul_y = 1.6 / 1080.0;
    
    vec2 blurCoordinates12 = textureCoordinate + vec2(0.0 * mul_x,-6.0 * mul_y);
    vec2 blurCoordinates13 = textureCoordinate + vec2(-4.0 * mul_x,-4.0 * mul_y);
    vec2 blurCoordinates14 = textureCoordinate + vec2(-6.0 * mul_x,0.0 * mul_y);
    vec2 blurCoordinates15 = textureCoordinate + vec2(-4.0 * mul_x,4.0 * mul_y);
    vec2 blurCoordinates16 = textureCoordinate + vec2(0.0 * mul_x,6.0 * mul_y);
    vec2 blurCoordinates17 = textureCoordinate + vec2(4.0 * mul_x,4.0 * mul_y);
    vec2 blurCoordinates18 = textureCoordinate + vec2(6.0 * mul_x,0.0 * mul_y);
    vec2 blurCoordinates19 = textureCoordinate + vec2(4.0 * mul_x,-4.0 * mul_y);
    
    float central;
    float gaussianWeightTotal;
    float sum;
    float sample;
    float distanceFromCentralColor;
    float gaussianWeight;
    
    float distanceNormalizationFactor = 3.6;
    
    central = texture2D(inputImageTexture1, textureCoordinate).g;
    gaussianWeightTotal = 0.2;
    sum = central * 0.2;
    
    sample = texture2D(inputImageTexture1, blurCoordinates0).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates1).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates2).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates3).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates4).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates5).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates6).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates7).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates8).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates9).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates10).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates11).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.08 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates12).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates13).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates14).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates15).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates16).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates17).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates18).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sample = texture2D(inputImageTexture1, blurCoordinates19).g;
    distanceFromCentralColor = min(abs(central - sample) * distanceNormalizationFactor, 1.0);
    gaussianWeight = 0.1 * (1.0 - distanceFromCentralColor);
    gaussianWeightTotal += gaussianWeight;
    sum += sample * gaussianWeight;
    
    sum = sum/gaussianWeightTotal;
    
    centralColor = texture2D(inputImageTexture1, textureCoordinate).rgb;
    
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
    
    smoothColor = mix(centralColor, smoothColor, smooth);
    
    gl_FragColor = lookup8x8(vec4(pow(smoothColor, vec3(0.96)), 1.0).rgb);
}
