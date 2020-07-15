#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec2 tex0size;
uniform vec2 tex1size;
void main(){
    float dis = distance(vec2(tex0size.x*0.75,tex0size.y*1.1),vec2(v_texcoord0.x*0.75,v_texcoord0.y*1.1));
    float dis01 = distance(vec2(tex0size.x,tex0size.y),vec2(v_texcoord0.x,v_texcoord0.y));
    vec4 rgba = texture2D(aTexture0,v_texcoord0);
    float size = tex1size.x;
    float sizey = tex1size.y;
    if(dis < size ){
        
    }else{
        float leng=sizey-size;
        leng = leng;
        float scale = ((leng-(dis-size))/(leng));
        scale = clamp(scale,0.0,1.0);
        rgba = rgba * scale ;
       // rgba = vec4(1.0,0.0,0.0,1.0);
    }

    gl_FragColor = rgba;
//    gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}


//float dis = distance(vec2(tex0size.x*0.5625,tex0size.y),vec2(v_texcoord0.x*0.5625,v_texcoord0.y));
//float dis01 = distance(vec2(tex0size.x,tex0size.y),vec2(v_texcoord0.x,v_texcoord0.y));
//vec4 rgba = texture2D(aTexture0,v_texcoord0);
//float size = tex1size.x*0.5625;
//float sizey = tex1size.y*0.5625;
//if(dis < size && dis01<(tex1size.x*0.8)){
//
//}else{
//    float leng=sizey-size;
//    leng = leng;
//    float scale = ((leng-(dis-size))/(leng));
//    scale = clamp(scale,0.0,1.0);
//    rgba = rgba * scale ;
//}
