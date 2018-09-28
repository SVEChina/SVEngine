#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D aTexture0;
uniform float u_mult_x;
uniform float u_mult_y;

varying vec2 v_texcoord0;

void main(){
    vec2 newTex = v_texcoord0*vec2(u_mult_x,u_mult_y);
    vec2 newInt = floor(newTex);
    vec2 newFloat = fract(newTex);
    vec2 newMode = fract(newInt/vec2(2.0,2.0));
    if(newMode.x>=0.5)
        newFloat.x = 1.0 - newFloat.x;
    if(newMode.y>=0.5)
        newFloat.y = 1.0 - newFloat.y;
    gl_FragColor = texture2D(aTexture0,newFloat);
}
