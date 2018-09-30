#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;

void main(){
    float whalf=0.5/255.0;
    vec4 textureColor = texture2D(aTexture0,v_texcoord0);
    float redCurveValue = texture2D(aTexture1, vec2(textureColor.r, 1.0)).r;
    float greenCurveValue = texture2D(aTexture1, vec2(textureColor.g, 1.0)).g;
    float blueCurveValue = texture2D(aTexture1, vec2(textureColor.b, 1.0)).b;
    gl_FragColor = vec4(redCurveValue,greenCurveValue,blueCurveValue,textureColor.a);
}
