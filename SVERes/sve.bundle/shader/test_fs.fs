#ifdef GL_ES
precision highp float;
#endif

varying vec4 v_pos;

void main(){
    float t_a = (50.0 - v_pos.x)/100.0;
    gl_FragColor = vec4(1.0,0.0,0.0, clamp(t_a,0.0,1.0) );
}
