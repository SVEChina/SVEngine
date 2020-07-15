#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_color;
varying vec2 v_texcoord0;
uniform sampler2D aTexture0;//源纹理
uniform float alpha;
void main(){
    vec4 color1 = texture2D(aTexture0,v_texcoord0);
    gl_FragColor = color1*alpha;
//    gl_FragColor = vec4(color1.rgb, 0.0);
}
