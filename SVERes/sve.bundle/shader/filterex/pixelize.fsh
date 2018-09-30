#ifdef GL_ES
precision mediump float;
#endif

uniform vec3                iResolution;
uniform sampler2D           aTexture0;
varying vec2                v_texcoord0;

#define S (iResolution.x / 6e1) // The cell size.

void mainImage(out vec4 c, vec2 p)
{
    c = texture2D(aTexture0, floor((p + .5) / S) * S / iResolution.xy);
}

void main() {
	mainImage(gl_FragColor, v_texcoord0*iResolution.xy);
}
