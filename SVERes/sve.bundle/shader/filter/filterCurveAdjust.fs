#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec3 curveArray[256]; // The array size is defined by "CURVE_PRECISION".
const float curvePrecision = 256.0; //The same to above.

void main()
{
    vec4 src = texture2D(aTexture0, v_texcoord0);
    gl_FragColor = vec4(curveArray[int(src.r * curvePrecision)].r,
                        curveArray[int(src.g * curvePrecision)].g,
                        curveArray[int(src.b * curvePrecision)].b,
                        src.a);
}
