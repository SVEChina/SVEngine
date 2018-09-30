#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;

void main()
{
    vec4 t_color = texture2D(aTexture0,v_texcoord0);
    vec4 t_mask = texture2D(aTexture1,v_texcoord0);
    gl_FragColor.rgb = t_color.rgb*(1.0-t_mask.a) + t_mask.rgb*t_mask.a;
    gl_FragColor.a = 1.0;
}
