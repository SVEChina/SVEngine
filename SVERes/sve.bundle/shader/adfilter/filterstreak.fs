#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
void main(){
    vec2 uv = v_texcoord0;
    float d = length(uv - vec2(0.5,0.5));
    
    // blur amount
    float blur = 0.007;
    
    // final color
    vec3 col;
    col.r = texture2D( aTexture0, vec2(uv.x+blur,uv.y) ).r;
    col.g = texture2D( aTexture0, uv ).g;
    col.b = texture2D( aTexture0, vec2(uv.x-blur,uv.y) ).b;
    
    // scanline
    float scanline = sin(uv.y*650.0)*0.08;
    col -= scanline;
    
    // vignette
    col *= 1.0 - d * 0.5;
    //
    gl_FragColor = vec4(col,1.0);
    
}
