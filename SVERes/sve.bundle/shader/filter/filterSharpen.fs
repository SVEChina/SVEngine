#ifdef GL_ES
precision highp float;
#endif

varying vec2 textureCoordinate;
varying vec2 leftTextureCoordinate;
varying vec2 rightTextureCoordinate;
varying vec2 topTextureCoordinate;
varying vec2 bottomTextureCoordinate;

varying float centerMultiplier;
varying float edgeMultiplier;

uniform sampler2D aTexture0;

void main()
{
    vec3 textureColor = texture2D(aTexture0, textureCoordinate).rgb;
    vec3 leftTextureColor = texture2D(aTexture0, leftTextureCoordinate).rgb;
    vec3 rightTextureColor = texture2D(aTexture0, rightTextureCoordinate).rgb;
    vec3 topTextureColor = texture2D(aTexture0, topTextureCoordinate).rgb;
    vec3 bottomTextureColor = texture2D(aTexture0, bottomTextureCoordinate).rgb;
    
    gl_FragColor = vec4((textureColor * centerMultiplier - (leftTextureColor * edgeMultiplier + rightTextureColor * edgeMultiplier + topTextureColor * edgeMultiplier + bottomTextureColor * edgeMultiplier)), texture2D(aTexture0, bottomTextureCoordinate).w);
}
