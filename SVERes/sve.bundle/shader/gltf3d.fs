#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;

void main() {
   gl_FragColor = texture2D(aTexture0,v_texcoord0);
   // gl_FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
    //gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}
