#ifdef GL_ES
precision mediump float;
#endif
//对比度
 varying vec2 v_texcoord0;
 uniform sampler2D aTexture0;
 uniform float intensity;
 
 void main()
{
    vec4 textureColor = texture2D(aTexture0, v_texcoord0);
    gl_FragColor = vec4(((textureColor.rgb - vec3(0.5)) * intensity + vec3(0.5)), textureColor.a);
}
 
