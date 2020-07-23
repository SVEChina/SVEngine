#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;

void main(){
    vec2 uv = v_texcoord0;
    vec4 color = texture2D(aTexture0, uv);
    float grayscale = color.r*0.299 + color.g*0.587 + color.b*0.114;
    gl_FragColor = vec4(grayscale, grayscale, grayscale, 1.0);
}
