#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float u_lerp;

void main(){
    vec3 ycoeff = vec3(0.256816, 0.504154, 0.0979137);
    vec4 texel = texture2D(aTexture0,v_texcoord0);
    float y = dot(texel.rgb, ycoeff) + 0.0625;
    gl_FragColor = texel*(1.0-u_lerp) + vec4(y,y,y,1.0)*u_lerp;
}
