#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;//源纹理
uniform vec4 u_strokecolor;
void main(){
    
    vec4 color1 = texture2D(aTexture0,v_texcoord0);
    vec4 color = color1*u_strokecolor;
    gl_FragColor = color;
    
}
