#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float saturation;
const vec3 saturationWeighting = vec3(0.2125, 0.7154, 0.0721);

void main(){
    vec4 rgba=texture2D(aTexture0,v_texcoord0).rgba;
    float luminance = dot(rgba.rgb, saturationWeighting);
    vec3 greyScaleColor = vec3(luminance);
    gl_FragColor=vec4(mix(greyScaleColor, rgba.rgb, saturation+1.0), rgba.w);
}
