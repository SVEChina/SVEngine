#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform sampler2D aTexture2;

void main(void){
    vec4 c =vec4(texture2D(aTexture0, v_texcoord0).r-16.0/255.0)*1.164;
    //We had put the U and V values of each pixel to the A and R,G,B
    //components of the texture respectively using GL_LUMINANCE_ALPHA.
    //Since U,V bytes are interspread in the texture, this is probably
    //the fastest way to use them in the shader
    vec4 u= vec4(texture2D(aTexture2, v_texcoord0).r - 0.5);
    vec4 v = vec4(texture2D(aTexture1, v_texcoord0).r - 0.5);
    c += u * vec4(0, -0.392, 2.017, 0);
    c += v * vec4(1.596, -0.813, 0, 0);
    c.a = 1.0;
    gl_FragColor = c;
}
