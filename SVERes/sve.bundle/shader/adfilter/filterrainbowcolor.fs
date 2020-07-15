#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float lerp;
void main(){
    float cycle = lerp*6.28;
    vec2 uv = v_texcoord0;
    float seperation = sin(cycle)/20.0;
    vec2 delta = vec2( seperation );
    vec4 color = texture2D(aTexture0,uv.xy);
    color.r = texture2D(aTexture0,uv.xy+delta ).r;
    color.b = texture2D(aTexture0,uv.xy-delta ).b;
    gl_FragColor = color;
}
