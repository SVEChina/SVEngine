#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;

uniform sampler2D aTexture0;

uniform float dis;
uniform float slope;
uniform vec3 hazeColor;

void main()
{
    float d = v_texcoord0.y * slope + dis;
    
    vec4 c = texture2D(aTexture0, v_texcoord0);
    
    c.rgb = (c.rgb - d * hazeColor.rgb) / (1.0 -d);
    
    gl_FragColor = c;
}
