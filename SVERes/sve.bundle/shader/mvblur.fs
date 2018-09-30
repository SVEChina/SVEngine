#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D aTexture0;
uniform vec2 u_texsize;
varying vec2 v_texcoord0;

void main(){
    vec2 texstep = vec2(1.0,1.0)/u_texsize;
    vec3 texcolor = vec3(0.0,0.0,0.0);;
    for(float i=-1;i<=1;i++){
        for(float j=-1;j<=1;j++){
            vec2 coord = clamp(v_texcoord0 + vec2(i,j)*texstep,0.0,1.0);
            texcolor = texcolor + texture2D(aTexture0,coord).rgb;
        }
    }
    gl_FragColor.rgb = texcolor/9.0;
    gl_FragColor.a = 1.0;
}
