#ifdef GL_ES
precision lowp float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float vibrance;

void main(){
    vec4 color = texture2D(aTexture0, v_texcoord0);
    float average = (color.r + color.g + color.b) / 3.0;
    float mx = max(color.r, max(color.g, color.b));
    float amt = (mx - average) * (-vibrance * 3.0);
    color.rgb = mix(color.rgb, vec3(mx), amt);
    gl_FragColor = color;
}
