#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_color;
varying vec2 v_texcoord0;
uniform sampler2D aTexture0;//源纹理
uniform sampler2D aTexture1;//源纹理
uniform float alpha;
void main(){
    vec4 color0 = texture2D(aTexture0,v_texcoord0);
    vec4 color1 = texture2D(aTexture1,v_texcoord0);
    vec4 color = vec4(color0.rgb*color1.rgb, color0.a*color1.r);
    gl_FragColor = color*alpha;
}
