uniform sampler2D srcTex;
uniform vec3 levels;

vec3 gammaCorrect(vec3 color, float gamma){
return pow(color, vec3(1.0/gamma));
}

vec3 levelRange(vec3 color, float minInput, float maxInput){
return min(
max(color - vec3(minInput), vec3(0.0)) / (vec3(maxInput) - vec3(minInput)),
vec3(1.0));
}

vec3 finalLevels(vec3 color, float minInput, float gamma, float maxInput){
return gammaCorrect(levelRange(color, minInput, maxInput), gamma);
}

void main(){
vec2 uv = gl_FragCoord.xy / vec2(512., 512.);

vec4 texture = texture2D(srcTex, uv) ;

// 注意 minInput和maxInput的取值范围为[0, 255]
vec3 adjustedLevels = finalLevels(texture.rgb, levels.x/255.0, levels.y, levels.z/255.0);

gl_FragColor = vec4(adjustedLevels,1.0);

}
