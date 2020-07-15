#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform float texelWidthOffset;//若为x模糊，可传入（1.0,0.0）  y模糊  （0.0,1.0）
uniform float texelHeightOffset;


void main (void){
    
    vec4 src=texture2D(aTexture1, v_texcoord0);
    vec4 sample[25];
    for (int i = 0; i < 25; i++){
        sample[i] = texture2D(aTexture0, v_texcoord0.xy + u_TextureCoordOffset[i]);
    }
    
    vec4 dst = (
                (1.0  * (sample[0] + sample[4]  + sample[20] + sample[24])) +
                (4.0  * (sample[1] + sample[3]  + sample[5]  + sample[9] + sample[15] + sample[19] + sample[21] + sample[23])) +
                (7.0  * (sample[2] + sample[10] + sample[14] + sample[22])) +
                (16.0 * (sample[6] + sample[8]  + sample[16] + sample[18])) +
                (26.0 * (sample[7] + sample[11] + sample[13] + sample[17])) +
                (41.0 * sample[12])
                ) / 273.0;
    gl_FragColor = vec4(dst.rgb, 1.0);
}
