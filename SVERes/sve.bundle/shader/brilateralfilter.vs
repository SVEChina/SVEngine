#ifdef GL_ES
precision mediump float;
#endif

attribute vec2 aPosition;
attribute vec2 aTexcoord0;
varying vec2 v_texcoord0;
varying vec2 blurCoordinates[9];    //blurCoordinates[GAUSSIAN_SAMPLES]
uniform float texelWidthOffset;
uniform float texelHeightOffset;
uniform vec2 texcoordClip;
const int GAUSSIAN_SAMPLES = 9;

void main()
{
    gl_Position = vec4(aPosition,0.0,1.0);
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
    //v_texcoord0 = aTexcoord0;
    
    // Calculate the positions for the blur
    int multiplier = 0;
    vec2 blurStep;
    vec2 singleStepOffset = vec2(texelWidthOffset, texelHeightOffset);
    
    for (int i = 0; i < GAUSSIAN_SAMPLES; i++)
    {
        multiplier = (i - ((GAUSSIAN_SAMPLES - 1) / 2));
        // Blur in x (horizontal)
        blurStep = float(multiplier) * singleStepOffset;
        blurCoordinates[i] = aTexcoord0.xy + blurStep;
    }
}
