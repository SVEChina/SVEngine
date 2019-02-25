precision highp float;
attribute vec4 aPosition;
attribute vec4 aTexcoord0;

uniform float imageWidthFactor;
uniform float imageHeightFactor;
uniform float sharpness;

varying vec2 textureCoordinate;
varying vec2 leftTextureCoordinate;
varying vec2 rightTextureCoordinate;
varying vec2 topTextureCoordinate;
varying vec2 bottomTextureCoordinate;

varying float centerMultiplier;
varying float edgeMultiplier;

void main()
{
    gl_Position = aPosition;
    
    vec2 widthStep = vec2(imageWidthFactor, 0.0);
    vec2 heightStep = vec2(0.0, imageHeightFactor);
    
    textureCoordinate = aTexcoord0.xy;
    leftTextureCoordinate = aTexcoord0.xy - widthStep;
    rightTextureCoordinate = aTexcoord0.xy + widthStep;
    topTextureCoordinate = aTexcoord0.xy + heightStep;
    bottomTextureCoordinate = aTexcoord0.xy - heightStep;
    
    centerMultiplier = 1.0 + 4.0 * sharpness;
    edgeMultiplier = sharpness;
}
