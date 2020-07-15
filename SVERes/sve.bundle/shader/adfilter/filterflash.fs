#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float lerp;
void main(){
    // final color
    float strength = sin(lerp*3.14);
    vec4 color = texture2D( aTexture0, v_texcoord0 );
    gl_FragColor = color+strength;
}
