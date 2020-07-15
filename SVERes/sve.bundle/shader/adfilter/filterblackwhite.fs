#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float mode;
void main(){
    vec3 pixelColor = texture2D(aTexture0, v_texcoord0).xyz;
    if (mode > 3.0)
    {
        // sRGB monitors grey scale coefficients
        float pixelGrey = dot(pixelColor, vec3(0.2126, 0.7152, 0.0722));
        pixelColor = vec3(pixelGrey);
    }
    else if (mode > 2.0)
    {
        // SD television grey scale coefficients
        float pixelGrey = dot(pixelColor, vec3(0.3, 0.59, 0.11));
        pixelColor = vec3(pixelGrey);
    }
    else if (mode > 1.0)
    {
        // naive grey scale conversion - average R,G and B
        float pixelGrey = dot(pixelColor, vec3(1.0/3.0));
        pixelColor = vec3(pixelGrey);
    }
    else
    {
        // color
    }
    gl_FragColor = vec4(pixelColor, 1.0);
}
