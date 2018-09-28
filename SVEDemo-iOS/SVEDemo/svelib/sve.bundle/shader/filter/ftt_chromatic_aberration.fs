#ifdef GL_ES
precision mediump float;
#endif

uniform float               iGlobalTime;
uniform sampler2D           aTexture0;
varying vec2                v_texcoord0;

void main() {
    float amount = 0.0;
    amount = (1.0 + sin(iGlobalTime*6.0)) * 0.5;
    amount *= 1.0 + sin(iGlobalTime*16.0) * 0.5;
    amount *= 1.0 + sin(iGlobalTime*19.0) * 0.5;
    amount *= 1.0 + sin(iGlobalTime*27.0) * 0.5;
    amount = pow(amount, 3.0);
    amount *= 0.05;
    vec2 uv = v_texcoord0;
    vec3 col;
    col.r = texture2D( aTexture0, vec2(uv.x+amount,uv.y) ).r;
    col.g = texture2D( aTexture0, uv ).g;
    col.b = texture2D( aTexture0, vec2(uv.x-amount,uv.y) ).b;
    col *= (1.0 - amount * 0.5);
    gl_FragColor = vec4(col,1.0);
}
