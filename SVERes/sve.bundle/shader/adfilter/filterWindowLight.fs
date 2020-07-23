#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;


void main(){
    vec2 uv = v_texcoord0;
    vec2 uv2 = vec2(uv.x, 1.0-uv.y);
    vec4 texColor1 = texture2D(aTexture0, uv);
    vec4 texColor2 = texture2D(aTexture1, uv2);
    gl_FragColor = texColor1+texColor2*0.4;
//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    
}
