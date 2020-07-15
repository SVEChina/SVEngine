#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;

const float repeats = 10.;

float nrand(in float x, in float y){
    return fract(sin(dot(vec2(x, y), vec2(12.9898, 78.233))) * 43758.5453);
}

void main(){
    vec2 uv = v_texcoord0;
    float bluramount = 0.01;
    vec4 blurred_image = vec4(0.0);
    for (float i = 0.; i < repeats; i++) {
        float t_sun = uv.x+uv.y;
        float t_rand = nrand(i,t_sun);
        float t_rand2 = nrand(i+2.,uv.x+uv.y+24.);
        vec2 q = vec2(sin(degrees(i/repeats*360.0)),cos(degrees(i/repeats*360.0)));
        vec2 uv2 = v_texcoord0 +(q*t_rand+bluramount)*bluramount;
        vec4 ttt = texture2D(aTexture0,uv2);
        blurred_image += ttt/2.0;
        uv2 = v_texcoord0 +(q*t_rand2+bluramount)*bluramount;
        ttt = texture2D(aTexture0,uv2);
        blurred_image += ttt/2.0;
    }
    blurred_image /= repeats;
    gl_FragColor = blurred_image;
}
