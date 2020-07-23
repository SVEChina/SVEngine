#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec2 tex0size;
uniform float time;

float rayStrength(vec2 raySource, vec2 rayRefDirection, vec2 coord, float seedA, float seedB, float speed)
{
    vec2 sourceToCoord = coord - raySource;
    float cosAngle = dot(normalize(sourceToCoord), rayRefDirection);
    return clamp(
        (0.45 + 0.15 * sin(cosAngle * seedA + time * speed)) +
        (0.3 + 0.2 * cos(-cosAngle * seedB + time * speed)),
        0.0, 1.0) *
        clamp((tex0size.x - length(sourceToCoord)) / tex0size.x, 0.5, 1.0);
}

void main(){
    vec2 uv = v_texcoord0;
    uv.y = 1.0 - uv.y;
    vec2 coord = tex0size*uv;
    
    // Set the parameters of the sun rays
    vec2 rayPos1 = vec2(tex0size.x * 0.7, tex0size.y * -0.4);
    vec2 rayRefDir1 = normalize(vec2(1.0, -0.116));
    const float raySeedA1 = 36.2214;
    const float raySeedB1 = 21.11349;
    const float raySpeed1 = 1.5;
    
    vec2 rayPos2 = vec2(tex0size.x * 0.8, tex0size.y * -0.6);
    vec2 rayRefDir2 = normalize(vec2(1.0, 0.241));
    const float raySeedA2 = 22.39910;
    const float raySeedB2 = 18.0234;
    const float raySpeed2 = 1.1;
    
    // Calculate the colour of the sun rays on the current fragment
    float rays1V = rayStrength(rayPos1, rayRefDir1, coord, raySeedA1, raySeedB1, raySpeed1);
    vec4 rays1 =
    vec4(rays1V, rays1V, rays1V, 1.0);
     
    float rays2V = rayStrength(rayPos2, rayRefDir2, coord, raySeedA2, raySeedB2, raySpeed2);
    vec4 rays2 =
    vec4(rays2V, rays2V, rays2V, 1.0);
    
    gl_FragColor = rays1 * 0.5 + rays2 * 0.4;
    
    // Attenuate brightness towards the bottom, simulating light-loss due to depth.
    // Give the whole thing a blue-green tinge as well.
    float brightness = 1.0 - (coord.y / tex0size.y);
    gl_FragColor.x *= 0.1 + (brightness * 0.8);
    gl_FragColor.y *= 0.3 + (brightness * 0.6);
    gl_FragColor.z *= 0.5 + (brightness * 0.5);
    
    vec2 uv2 = v_texcoord0;
    vec4 texColor = texture2D(aTexture0, uv2);
    gl_FragColor *= texColor*2.0;
//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    
}
