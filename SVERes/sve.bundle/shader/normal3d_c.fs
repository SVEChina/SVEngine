//#version 150
varying vec4 v_color;
varying vec3 v_Normal;
varying vec2 v_texcoord0;
varying vec3 v_FragPos;
uniform sampler2D aTexture0;
uniform vec3 u_ambient_color;
uniform float u_ambientStrength;
uniform vec3 u_lightPos;
uniform vec3 u_lightColor;
uniform vec3 u_viewPos;
void main(){
    //ambient
    vec3 ambient = vec3(u_ambientStrength * u_ambient_color);
    //
    vec3 lightPos = u_lightPos;
    vec3 lightColor = u_lightColor;
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(lightPos - v_FragPos);
    //diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    //specular
    float specularStrength = 1.0;
    vec3 viewPos = u_viewPos;
    vec3 viewDir = normalize(viewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;
    //
    vec4 color = texture2D(aTexture0,v_texcoord0);
    vec4 result = (ambient + diffuseColor + specular)*color*v_color;
    gl_FragColor = result;
}
