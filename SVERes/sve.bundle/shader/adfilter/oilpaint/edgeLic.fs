#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform vec2 tex0size;
uniform vec2 tex1size;
const int aLength = 4;
const float sigma = 0.1;
const float aStep = 1.0;
const float edgeThreshold = 0.0;
//void main(){
//
////    if( VectorField[id.xy].w <  edgeThreshold ){
////         Result[id.xy] = float4(1,1,1,1);
////         return;
////     }
//
//    vec2 uv = v_texcoord0;
//    vec3 sum =  texture2D(aTexture0, uv).rgb;
//    float weight = 1.0;
//    float k;
//    vec2 v = (texture2D(aTexture1, uv).xy);
//    v *= 1.0/tex0size;
//    vec2 pt = vec2(uv.x + 0.5/tex0size.x, uv.y + 0.5/tex0size.y) + v*aStep;
//    vec2 v2;
//    vec2 p;
//     for(int i=1;i<aLength;i++){
//         float t_i = float(i);
//         p = vec2(clamp(pt.x, 0.0, 1.0), clamp(pt.y, 0.0, 1.0));
//         k = exp(-t_i*t_i*sigma);
//         vec3 color = texture2D(aTexture0, p).rgb;
//         sum += color*k;
//         weight += k;
//
//         v2 = texture2D(aTexture1, p).xy;
//         if( v2.x ==0.0 && v2.y ==0.0 ){
//             break;
//         }
//         v = sign(dot(v,v2)) * v2;
//         pt += v*aStep;
//    }
//
//
//     v = -texture2D(aTexture1, uv).xy;
//     v *= 1.0/tex0size;
//     pt = vec2(uv.x + 0.5/tex0size.x,uv.y + 0.5/tex0size.y)+v*aStep;
//     for(int i=1;i<aLength;i++){
//         float t_i = float(i);
//         p = vec2(clamp(pt.x, 0.0, 1.0), clamp(pt.y, 0.0, 1.0));
//         k = exp(-t_i*t_i*sigma);
//         vec3 color = texture2D(aTexture0, p).rgb;
//         sum += color*k;
//         weight += k;
//
//         v2 = texture2D(aTexture1, p).xy;
//         if( v2.x ==0.0 && v2.y ==0.0 ){
//             break;
//         }
//         v = sign(dot(v,v2)) * v2;
//         pt += v*aStep;
//    }
//    sum /= weight;
//    float v_w = texture2D(aTexture1, uv).w;
//    gl_FragColor = vec4(sum*exp(-v_w),1.0);
////    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//}
void main(){
    
    vec2 uv = v_texcoord0;
    vec3 col = texture2D(aTexture0, uv).rgb;
    vec2 v = (texture2D(aTexture1, uv).xy - 0.5) * 2.0;
    //    vec2 velocity = vec2(cos((uv.x - 2.0*time)), sin((uv.x - 2.0*time)));
    //    v *= velocity;
    v *= 1./tex0size;
    int w = 0;
    vec2 st0 = uv;
    for(int i = 0; i < aLength; i++) {
        st0 += v;
        vec3 n = texture2D(aTexture0, st0).rgb;
        col += n;
        w++;
    }

    vec2 st1 = uv;
    for(int i = 0; i < aLength; i++) {
        st1 -= v;
        vec3 n = texture2D(aTexture0, st1).rgb;
        col += n;
        w++;
    }
    col /= float(w);
    gl_FragColor = vec4( col, 1. );
}
