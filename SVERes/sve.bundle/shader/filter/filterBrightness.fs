#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float intensity;

void main()
{
    vec4 src = texture2D(aTexture0, v_texcoord0);
    
    float fac = 1.0 / intensity;
    float fac2 = 1.0 - fac;
    vec3 tmp1 = fac2 - src.rgb;
    vec3 tmp2 = sqrt(fac2 * fac2 + (4.0 * fac) * src.rgb);
    
    src.rgb = tmp1 + tmp2 * (step(0.0, intensity) * 2.0 - 1.0);
    
    gl_FragColor = src;
}
