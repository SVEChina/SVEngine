#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float intensity;
void main(){
    vec3 pixelColor = texture2D(aTexture0, v_texcoord0).xyz;
    gl_FragColor = vec4(pixelColor*intensity, 1.0);
}
