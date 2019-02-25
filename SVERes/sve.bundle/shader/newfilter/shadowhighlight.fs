#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float shadows;
uniform float highlights;
const vec3 luminanceWeighting = vec3(0.3, 0.3, 0.3);
void main(){
    float t_shadows=abs(shadows);
    float t_highlights=0.0;
    if(highlights>0.0){
         t_highlights=1.0-highlights;
    }else{
        t_highlights=1.0+highlights;
    }
    vec4 rgba=texture2D(aTexture0, v_texcoord0);
    float luminance = dot(rgba.rgb, luminanceWeighting);
    float shadow=0.0;
    float highlight=0.0;
     shadow = clamp((pow(luminance, 1.0/(t_shadows+1.0)) + (-0.76)*pow(luminance, 2.0/(t_shadows+1.0))) - luminance, 0.0, 1.0);
     highlight = clamp((1.0 - (pow(1.0-luminance, 1.0/(2.0-t_highlights)) + (-0.8)*pow(1.0-luminance, 2.0/(2.0-t_highlights)))) - luminance, -1.0, 0.0);
    if(shadows<=0.0){
        shadow=-1.0*shadow;
    }
    
    if(highlights>0.0){
        highlight=-1.0*highlight;
    }
    
    float flagh=step(0.0,highlights);
    vec3 result = vec3(0.0, 0.0, 0.0) + ((luminance +shadow+highlight) - 0.0) * ((rgba.rgb - vec3(0.0, 0.0, 0.0))/(luminance - 0.0));
    gl_FragColor=vec4(result.rgb, rgba.a);
}
