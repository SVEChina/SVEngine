#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float brightness;//-1 to 1
uniform float contrast;//-1 to 1

void main(){
    vec4 rgba= texture2D(aTexture0,v_texcoord0);
    float k = tan( (45.0 + 44.0*contrast) / 180.0 * 3.1415926 );
    gl_FragColor=(rgba - 0.5 * (1.0 - brightness)) * k + 0.5 * (1.0 + brightness);
}
