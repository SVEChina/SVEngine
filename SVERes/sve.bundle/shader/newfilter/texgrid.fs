#ifdef GL_ES
precision lowp float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float row;
uniform float col;

void main(){
    vec2 t_coord = vec2(10.0,10.0)*v_texcoord0;
    gl_FragColor = texture2D(aTexture0,t_coord);
}
