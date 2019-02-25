#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_color;
varying vec2 v_texcoord0;
uniform sampler2D aTexture0;//源纹理
uniform int blendmode;

float overlayBlendMode(float basePixel, float blendPixel) {
    if (basePixel < 0.5) {
        return (2.0 * basePixel * blendPixel);
    } else {
        return (1.0 - 2.0 * (1.0 - basePixel) * (1.0 - blendPixel));
    }
}

void main(){
    
    if (blendmode == 0) {
        //正常
        vec4 color1 = texture2D(aTexture0,v_texcoord0);
        gl_FragColor = v_color*color1;
    }else if(blendmode == 1){
        //叠加
        vec4 color1 = texture2D(aTexture0,v_texcoord0);
        float gray = color1.r*0.299 + color1.g*0.587 + color1.b*0.114;
//        float r = overlayBlendMode(gray, color1.r);
//        float g = overlayBlendMode(gray, color1.g);
//        float b = overlayBlendMode(gray, color1.b);
//        float a = overlayBlendMode(gray, color1.a);
//        vec4 color = vec4(r,g,b,a);
        vec4 color = vec4(color1.r, color1.g, color1.b, gray);
        gl_FragColor = v_color*color;
    }else if(blendmode == 2){
        //正片叠底
        vec4 color1 = texture2D(aTexture0,v_texcoord0);
        float gray = color1.r*0.299 + color1.g*0.587 + color1.b*0.114;
        vec4 color = vec4(color1.r, color1.g, color1.b, gray);
        gl_FragColor = v_color*color;
    }else if(blendmode == 3){
        //滤色
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
