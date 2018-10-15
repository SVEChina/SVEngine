#ifdef GL_ES
precision mediump float;
varying highp vec2 v_texcoord0;
#else
varying vec2 v_texcoord0;
#endif

uniform sampler2D aTexture0;    //l texture
uniform sampler2D aTexture1;    //l-r texture

void main (void){
    vec4 c =vec4(texture2D(aTexture0, v_texcoord0).r-16.0/255.0)*1.164;
//    vec4 u= vec4(texture2D(aTexture1, v_texcoord0).r - 0.5);
//    vec4 v = vec4(texture2D(aTexture1, v_texcoord0).a - 0.5);
//    c += v * vec4(1.596, -0.813, 0, 0);
//    c += u * vec4(0, -0.392, 2.017, 0);
//    c.a = 1.0;
    //We finally set the RGB color of our pixel
    gl_FragColor = c;//vec4(0.0,1.0,1.0,1.0);
}
