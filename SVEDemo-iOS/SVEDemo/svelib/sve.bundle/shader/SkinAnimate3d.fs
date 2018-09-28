#ifdef GL_ES
precision mediump float;
#endif

void main(){
    gl_FragColor.rgba =texture2D(aTexture0,v_texcoord0);
}
