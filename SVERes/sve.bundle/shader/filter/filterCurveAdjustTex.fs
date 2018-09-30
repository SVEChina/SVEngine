#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D curveTexture; //We do not use sampler1D because GLES dosenot support that.

void main()
{
    vec4 src = texture2D(aTexture0, v_texcoord0);
    gl_FragColor = vec4(texture2D(curveTexture, vec2(src.r, 0.0)).r,
                        texture2D(curveTexture, vec2(src.g, 0.0)).g,
                        texture2D(curveTexture, vec2(src.b, 0.0)).b,
                        src.a);
}
