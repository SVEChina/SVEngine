#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float lerp;
void main(){
    vec2 uv = v_texcoord0;
    float d = length(uv - vec2(0.5,0.5));
    
    // blur amount
    float blur = 0.007;
    
    // final color
    float cycle = lerp*6.28;
    float seperation = sin(cycle)/20.0;
    vec2 delta = vec2( seperation+blur, seperation );
    vec3 col;
    col.r = texture2D( aTexture0, vec2(fract(uv.x+delta.x),uv.y+delta.y) ).r;
    col.g = texture2D( aTexture0, uv ).g;
    col.b = texture2D( aTexture0, vec2(fract(uv.x-delta.x),uv.y+delta.y) ).b;
    
    // scanline
    float scanline = sin(uv.y*650.0)*0.08;
    col -= scanline;
    
    // vignette
    col *= 1.0 - d * 0.5;
    //
    gl_FragColor = vec4(col,1.0);
    
}
