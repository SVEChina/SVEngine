#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_color;
varying vec2 v_texcoord0;
uniform sampler2D aTexture0;//源纹理
void main(){
    
    vec4 color1 = texture2D(aTexture0,v_texcoord0);
    vec4 color2 = v_color;
    color2.a=color1.a;
    gl_FragColor = color2;
    
}
