#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
varying vec2 v_texcoord0;
uniform vec2 tex0size;

void main(){
    vec4 textureColor =texture2D(aTexture0, v_texcoord0);
    float blueColor = textureColor.b * 63.0;
    vec2 quad1;
    quad1.y = floor(floor(blueColor) / 8.0);
    quad1.x = floor(blueColor) - (quad1.y * 8.0);
    vec2 quad2;
    quad2.y = floor(ceil(blueColor) /8.0);
    quad2.x = ceil(blueColor) - (quad2.y * 8.0);
    vec2 texPos1;
    texPos1.x = (quad1.x * 1.0/8.0)+ 0.5/512.0 + ((1.0/8.0 - 1.0/512.0) * textureColor.r);
    texPos1.y = (quad1.y * 1.0/8.0) + ((1.0/8.0 - 1.0/512.0) * textureColor.g);
    vec2 texPos2;
    texPos2.x = (quad2.x * 1.0/8.0)+ 0.5/512.0 + ((1.0/8.0 - 1.0/512.0) * textureColor.r);
    texPos2.y = (quad2.y * 1.0/8.0)+ 0.5/512.0 + ((1.0/8.0 - 1.0/512.0) * textureColor.g);
    vec4 newColor1 = texture2D(aTexture1, texPos1);
    vec4 newColor2 = texture2D(aTexture1, texPos2);
    vec4 newColor = mix(newColor1, newColor2, fract(blueColor));
    gl_FragColor=mix(textureColor, vec4(newColor.rgb, textureColor.w),1.0);
}
