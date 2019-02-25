#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float curveArrayRGB[256]; // The array size is defined by "CURVE_PRECISION".

const float curvePrecision = 256.0; //The same to above.

void main()
{
    vec4 src = texture2D(aTexture0, v_texcoord0);
    gl_FragColor = vec4(curveArrayRGB[int(src.r * curvePrecision)],
                        curveArrayRGB[int(src.g * curvePrecision)],
                        curveArrayRGB[int(src.b * curvePrecision)],
                        src.a);
}
