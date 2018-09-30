//lvjing4

#ifdef GL_ES
precision lowp  float;
#endif

 varying highp vec2 v_texcoord0;

 uniform sampler2D aTexture0;
 uniform sampler2D aTexture1;  //process
 uniform sampler2D aTexture2;  //blowout
 uniform sampler2D aTexture3;  //contrast
 uniform sampler2D aTexture4;  //luma
 uniform sampler2D aTexture5;  //screen
 uniform int disableEffect;

 mat3 saturateMatrix = mat3(
                        1.105150,-0.044850,-0.046000,
                        -0.088050,1.061950,-0.089200,
                        -0.017100,-0.017100,1.132900);
 vec3 luma = vec3(.3, .59, .11);
 void main()
 {
     vec3 texel = texture2D(aTexture0, v_texcoord0).rgb;
     vec2 lookup;
     lookup.y = 0.5;
     lookup.x = texel.r;
     texel.r = texture2D(aTexture1, lookup).r;
     lookup.x = texel.g;
     texel.g = texture2D(aTexture1, lookup).g;
     lookup.x = texel.b;
     texel.b = texture2D(aTexture1, lookup).b;

     texel = saturateMatrix * texel;

     vec2 tc = (2.0 * v_texcoord0) - 1.0;
     float d = dot(tc, tc);
     vec3 sampled;
     lookup.y = 0.5;
     lookup.x = texel.r;
     sampled.r = texture2D(aTexture2, lookup).r;
     lookup.x = texel.g;
     sampled.g = texture2D(aTexture2, lookup).g;
     lookup.x = texel.b;
     sampled.b = texture2D(aTexture2, lookup).b;
     float value = smoothstep(0.0, 1.0, d);
     texel = mix(sampled, texel, value);

     lookup.x = texel.r;
     texel.r = texture2D(aTexture3, lookup).r;
     lookup.x = texel.g;
     texel.g = texture2D(aTexture3, lookup).g;
     lookup.x = texel.b;
     texel.b = texture2D(aTexture3, lookup).b;


     lookup.x = dot(texel, luma);
     texel = mix(texture2D(aTexture4, lookup).rgb, texel, .5);

     lookup.x = texel.r;
     texel.r = texture2D(aTexture5, lookup).r;
     lookup.x = texel.g;
     texel.g = texture2D(aTexture5, lookup).g;
     lookup.x = texel.b;
     texel.b = texture2D(aTexture5, lookup).b;

     vec4 tmp_FragColor = vec4(texel, 1.0);
     gl_FragColor = vec4(((tmp_FragColor.rgb - vec3(0.5)) * 0.7 + vec3(0.5)), tmp_FragColor.w);

}
