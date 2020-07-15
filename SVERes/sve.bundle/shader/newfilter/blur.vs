#ifdef GL_ES
precision highp float;
#endif

attribute vec2 aPosition;
attribute vec2 aColor0;
attribute vec2 aTexcoord0;
uniform float m_smooth;

varying vec2 v_texcoord0;
uniform vec2 texcoordClip;

const int GAUSSIAN_SAMPLES = 9;
uniform float m_width;
uniform float m_height;
varying vec2 blurCoordinates[GAUSSIAN_SAMPLES];

void main(){
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
    gl_Position = vec4(aPosition.xy,0.0,1.0);
    vec2 blurStep;
    int multiplier=0;
    vec2 singleStepOffset = vec2(m_width, m_height);
    for (int i = 0; i < GAUSSIAN_SAMPLES; i++){
        multiplier = (i - ((GAUSSIAN_SAMPLES - 1) / 2));
        blurStep = float(multiplier) * (singleStepOffset*m_smooth);
        blurCoordinates[i] = v_texcoord0.xy + blurStep;
    }
}

