precision highp float;

attribute vec2 aPosition;
attribute vec2 aTexcoord0;

const int GAUSSIAN_SAMPLES = 9;

uniform float texelWidthOffset;//(float)1/width
uniform float texelHeightOffset;//(float)1/height

varying vec2 v_texcoord0;

varying vec2 blurCoordinates[GAUSSIAN_SAMPLES];

uniform vec2 texcoordClip;

void main()
{
    gl_Position = vec4(aPosition.xy,0.0,1.0);
//    v_texcoord0 = aTexcoord0;
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip  + 0.5;

//    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip  + 0.5;
    int multiplier = 0;
    vec2 blurStep;
    vec2 singleStepOffset = vec2(texelWidthOffset, texelHeightOffset);
    
    for (int i = 0; i < GAUSSIAN_SAMPLES; i++)
    {
        multiplier = (i - ((GAUSSIAN_SAMPLES - 1) / 2));
        // Blur in x (horizontal)
        blurStep = float(multiplier) * singleStepOffset;
        blurCoordinates[i] = v_texcoord0.xy + blurStep;
    }
    
}