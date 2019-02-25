#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D aTexture0;
uniform float u_lerp;
uniform vec4 u_color;

varying vec2 v_texcoord0;

void main(){
     vec3 texcolor = texture2D(aTexture0,v_texcoord0).rgb;
     gl_FragColor.rgb = texcolor*u_lerp + u_color.rgb*(1.0-u_lerp);
     gl_FragColor.a = 1.0;
}
