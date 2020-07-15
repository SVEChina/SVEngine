#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
varying float v_lerp;
uniform sampler2D aTexture0;
void main(){
    float seperation = sin(1.57*v_lerp)*0.01;
    vec2 uv = v_texcoord0;
    vec4 red = texture2D(aTexture0, uv+seperation);
    vec4 green = texture2D(aTexture0, uv-seperation);
    vec4 blue = texture2D(aTexture0, uv);
    gl_FragColor = vec4(red.r, green.g, blue.b, 1.0);
}
