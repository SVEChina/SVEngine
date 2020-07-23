#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(92.,80.))) +
  cos(dot(co.xy ,vec2(2.0,62.))) * 2.1);
}

void main(){
    float amt = 0.004;
    vec2 uv = v_texcoord0;
    vec2 rnd = vec2(rand(uv.xy),rand(uv.xy));
    gl_FragColor = texture2D(aTexture0, uv.xy+rnd*amt);
    
}
