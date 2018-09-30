#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_color;
varying vec2 v_texcoord0;
varying vec2 v_texcoord1;
uniform sampler2D aTexture0;//源纹理
uniform sampler2D aTexture1;//目标纹理
uniform int blendmode;
void main(){
    
    
    if (blendmode == 0) {
        //正常
        vec4 color1 = texture2D(aTexture0,v_texcoord0);
        gl_FragColor = v_color*color1;
    }else if(blendmode == 1){
        //SP_BLEND_MODE_ADDITIVE
        
    }else if(blendmode == 2){
        //正片叠底
        
    }else if(blendmode == 3){
        //滤色
//        vec4 blendTex = texture2D(aTexture0,v_texcoord0);
//        vec4 renderTex = texture2D(aTexture1,v_texcoord1);
//        vec4 blendedScreen = (1.0 - ((1.0 - renderTex) * (1.0 - blendTex)));
//        gl_FragColor = renderTex;
        vec4 color1 = texture2D(aTexture0,v_texcoord0);
        float gray = color1.r*0.299 + color1.g*0.587 + color1.b*0.114;
        vec4 color = vec4(color1.r, color1.g, color1.b, gray);
        gl_FragColor = v_color*color;
    }else {
        //其他
        vec4 color1 = texture2D(aTexture0,v_texcoord0);
        gl_FragColor = v_color*color1;
    }
    
}
