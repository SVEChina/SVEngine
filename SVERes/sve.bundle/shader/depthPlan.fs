#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform vec2 tex0size;
varying float v_depth;
varying vec4 v_posLightSpace;

void main(){
    
    vec2 texelSize = 1.0 / tex0size;
    vec3 projCoords = v_posLightSpace.xyz / v_posLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture2D(aTexture0, projCoords.xy).r;
    vec4 plan = texture2D(aTexture1,v_texcoord0);
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float closestDepth = texture2D(aTexture0, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += closestDepth;
        }
    }
    shadow /= 9.0;
    
    if(shadow==1.0){
        gl_FragColor = plan;
    }else{
        gl_FragColor = vec4(0.35,0.35,0.35,v_depth);
    }
    

}
